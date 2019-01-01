//  Real Time Clock (RTC) Functions to keep track of current time, and wake from low power mode.
//  Based on https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/rtc/main.c
#include <string.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <logger.h>
#include "bluepill.h"
#ifdef LIBOPENCM3_RCC_LEGACY  //  If using older version of libopencm3 (PlatformIO)...
#define RCC_LSE LSE           //  Fast Oscillator is named LSE.
#define RCC_HSE HSE           //  Slow Oscillator is named HSE.
#endif  //  LIBOPENCM3_RCC_LEGACY

static void custom_rtc_awake_from_off(enum rcc_osc clock_source);

//  Select Oscillator for the realtime clock: 
//  RCC_HSE: 62.5 kHz, fastest oscillator, doesn't work in Stop or Standby Low Power mode. 
//  RCC_LSE: 32.768 kHz, slowest oscillator, works in Stop or Standby Low Power mode. 
//  RCC_LSI: 40 kHz, works in Stop or Standby Low Power mode. 
//  We choose RCC_LSE because we need to wake up in Low Power mode.

#define USE_RCC_LSE  //  If using Low Power Mode...
#ifdef USE_RCC_LSE
const rcc_osc clock_source = RCC_LSE;
#else
const rcc_osc clock_source = RCC_HSE;
#endif  //  USE_RCC_LSE

//  Set the clock prescaling value, so that we will get a tick interrupt every 1 millisecond. Dependent on LSE or HSE clock selection.
#ifdef USE_RCC_LSE
const uint32_t prescale = 32;        //  For RCC_LSE: 1 millisecond tick (should actually be 32.7)
// const uint32_t prescale = 327;    //  For RCC_LSE: 10 millisecond tick
#else
const uint32_t prescale = 62;        //  For RCC_HSE: 1 millisecond tick (should actually be 62.5)
// const uint32_t prescale = 62500;  //  For RCC_HSE: 1 second tick
#endif  //  USE_RCC_LSE

static void (*tickFunc)(void) = NULL;     //  This is the tick function we will call every millisecond.  
static void (*alarmFunc)(void) = NULL;    //  This is the alarm function we will upon getting an RTC alarm.
static volatile uint32_t tickCount = 0;   //  Number of millisecond ticks elapsed.
static volatile uint32_t alarmCount = 0;  //  Number of alarms elapsed.

static void rtc_setup(void) {
	//  Setup RTC interrupts for tick and alarm wakeup.
	rcc_enable_rtc_clock();
	rtc_interrupt_disable(RTC_SEC);
	rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_disable(RTC_OW);

	//  Note: Older versions of rtc_awake_from_off() and rtc_auto_awake() cause qemu to crash with error
	//  "hardware error: you are must enter to configuration mode for write in any registre" in hw\timer\stm32_rtc.c
	debug_println("rtc awake..."); debug_flush(); //  rtc_awake_from_off() fails on qemu.
#ifdef AUTO_AWAKE	
	//  From: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32vl-discovery/rtc/rtc.c
	//  rtc_auto_awake(): If the RTC is pre-configured just allow access, don't reconfigure.
	//  Otherwise enable it with the clock source and set the prescale value.
	rtc_auto_awake(clock_source, prescale);
#else
	//  rtc_auto_awake() will not reset the RTC when you press the RST button.
	//  It will also continue to count while the MCU is held in reset. If
	//  you want it to reset, use custom_rtc_awake_from_off()
	custom_rtc_awake_from_off(clock_source);  //  This will enable RTC.
	rtc_set_prescale_val(prescale);
#endif  //  AUTO_AWAKE
	debug_println("rtc awake ok"); debug_flush(); //  rtc_awake_from_off() fails on qemu.
	
	rtc_set_counter_val(0);              //  Start counting millisecond ticks from 0.
	rtc_set_alarm_time((uint32_t) -1);   //  Reset alarm to -1 or 0xffffffff so we don't trigger now.
	exti_set_trigger(EXTI17, EXTI_TRIGGER_RISING);  //  Enable alarm wakeup via the interrupt.
	exti_enable_request(EXTI17);

	nvic_enable_irq(NVIC_RTC_IRQ);        //  Enable RTC tick interrupt processing.
	nvic_enable_irq(NVIC_RTC_ALARM_IRQ);  //  Enable RTC alarm wakeup interrupt processing.

	cm_disable_interrupts();
	rtc_clear_flag(RTC_SEC);
	rtc_clear_flag(RTC_ALR);
	rtc_clear_flag(RTC_OW);
	rtc_interrupt_enable(RTC_SEC);  //  Allow RTC to generate tick interrupts.
	rtc_interrupt_enable(RTC_ALR);  //  Allow RTC to generate alarm interrupts.
	cm_enable_interrupts();
}

void platform_start_timer(void (*tickFunc0)(void), void (*alarmFunc0)(void)) {
    //  Start the STM32 Timer to generate interrupt ticks to perform task switching.
  	tickFunc = tickFunc0;    //  Allow tickFunc to be modified at every call to platform_start_timer().
  	alarmFunc = alarmFunc0;  //  Allow alarmFunc to be modified at every call to platform_start_timer().
	
	//  But system timer will only be started once.
	static bool timerStarted = false;
	if (timerStarted) { return; }
	timerStarted = true;
	debug_println("platform_start_timer"); ////
	rtc_setup();
}

void platform_set_alarm(uint32_t millisec) {
	//  Set alarm for millisec milliseconds from now.
	//  debug_print("alm <"); debug_print_unsigned(millisec / 1000); ////
	if (!alarmFunc) { debug_print("alm? "); } ////
	volatile uint32_t now = rtc_get_counter_val();

	//  Not documented, but you must disable write protection else the alarm time will not be set and rtc_exit_config_mode() will hang.
	pwr_disable_backup_domain_write_protect();
	rtc_set_alarm_time(now + millisec);

#ifdef NOTUSED
	//  Must disable interrupts otherwise rtc_exit_config_mode() will hang after setting alarm. 
	cm_disable_interrupts();

	//  Not documented, but you must disable write protection else the alarm time will not be set.
	pwr_disable_backup_domain_write_protect();

	rtc_interrupt_disable(RTC_SEC);
	rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_disable(RTC_OW);
	
	rtc_set_alarm_time(now + millisec);
	exti_set_trigger(EXTI17, EXTI_TRIGGER_RISING);  //  Enable alarm wakeup via the interrupt.
	exti_enable_request(EXTI17);

	nvic_enable_irq(NVIC_RTC_IRQ);        //  Enable RTC tick interrupt processing.
	nvic_enable_irq(NVIC_RTC_ALARM_IRQ);  //  Enable RTC alarm wakeup interrupt processing.

	rtc_clear_flag(RTC_SEC);
	rtc_clear_flag(RTC_ALR);
	rtc_clear_flag(RTC_OW);
	rtc_interrupt_enable(RTC_SEC);  //  Allow RTC to generate tick interrupts.
	rtc_interrupt_enable(RTC_ALR);  //  Allow RTC to generate alarm interrupts.

	cm_enable_interrupts();
#endif  //  NOTUSED

	//  debug_print("> "); ////
	//  TODO: rtc_enable_alarm()
}

volatile uint32_t platform_get_alarm(void) {
	//  Get alarm time.
	return rtc_get_alarm_val();
}

void rtc_isr(void) {
	//  Interrupt Service Routine for RTC Tick, Alarm, Overflow.  Don't call any I/O functions here.
	if (rtc_check_flag(RTC_SEC)) {
		//  We hit an RTC tick interrupt.
		rtc_clear_flag(RTC_SEC);
		tickCount++;
		//  Call the tick function.
		if (tickFunc != NULL) { tickFunc(); }
		return;
	}
#ifdef NOTUSED  //  Alarm handled by rtc_alarm_isr()
	if (rtc_check_flag(RTC_ALR)) {
		//  We hit an RTC alarm interrupt.
		rtc_clear_flag(RTC_ALR);
		alarmCount++;
		//  Call the alarm function.
		if (alarmFunc != NULL) { alarmFunc(); }
		return;
	}
#endif  //  NOTUSED
}

void rtc_alarm_isr(void) {
	//  Interrupt Service Routine for RTC Alarm Wakeup.  Don't call any I/O functions here.
	//  The RTC alarm appears as EXTI 17 which must be reset independently of the RTC alarm flag.
	exti_reset_request(EXTI17);
	rtc_clear_flag(RTC_ALR);
	alarmCount++;
	//  Call the alarm function.
	if (alarmFunc != NULL) { alarmFunc(); }
}

volatile uint32_t millis(void) {
	//  Return the number of millisecond ticks since startup.
	//  Compatible with Arduino's millis() function.
	//  TODO: Compensate for clock slowdown because we truncated RCC_LSE 32.768 kHz to 32.
	return rtc_get_counter_val();  //  More accurate, uses hardware counters.
	// return tickCount;  //  Less accurate, excludes ARM Semihosting time. 
}

volatile uint32_t platform_alarm_count(void) {
	//  Return the number of alarms triggered since startup.
	return alarmCount;  //  For testing whether alarm ISR was called.
}

volatile uint32_t platform_tick_count(void) {
	//  Return the number of alarms triggered since startup.  Less accurate, excludes ARM Semihosting time.
	return tickCount;  //  For testing whether tick ISR was called.
}

//  Latest version of rtc_awake_from_off() from https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/f1/rtc.c
//  PlatformIO uses an older version that crashes QEMU.  libopencm3 versions released 31 Mar 2017 onwards have this fix.
//  See https://github.com/libopencm3/libopencm3/commit/808cd44a41797b98102151c35f1c858aa53fdeb5#diff-53d037e30430840081cbd1ae8eeab166

/*---------------------------------------------------------------------------*/
/** @brief RTC Set Operational from the Off state.
Power up the backup domain clocks, enable write access to the backup domain,
select the clock source, clear the RTC registers and enable the RTC.
@param[in] clock_source ::rcc_osc. RTC clock source. Only the values HSE, LSE
    and LSI are permitted.
*/
static void custom_rtc_awake_from_off(enum rcc_osc clock_source)
{
	uint32_t reg32;

	/* Enable power and backup interface clocks. */
	rcc_periph_clock_enable(RCC_PWR);
	rcc_periph_clock_enable(RCC_BKP);

	/* Enable access to the backup registers and the RTC. */
	pwr_disable_backup_domain_write_protect();

	/* Set the clock source */
	rcc_set_rtc_clock_source(clock_source);

	/* Clear the RTC Control Register */
	RTC_CRH = 0;
	RTC_CRL = 0;

	/* Enable the RTC. */
	rcc_enable_rtc_clock();

	/* Clear the Registers */
	rtc_enter_config_mode();
	RTC_PRLH = 0;
	RTC_PRLL = 0;
	RTC_CNTH = 0;
	RTC_CNTL = 0;
	RTC_ALRH = 0xFFFF;
	RTC_ALRL = 0xFFFF;
	rtc_exit_config_mode();

	/* Wait for the RSF bit in RTC_CRL to be set by hardware. */
	RTC_CRL &= ~RTC_CRL_RSF;
	while ((reg32 = (RTC_CRL & RTC_CRL_RSF)) == 0);
}

#ifdef NOTUSED
	//  Custom version of custom_rtc_exit_config_mode() and custom_rtc_set_alarm_time() from https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/f1/rtc.c
	//  This fixes the hanging in custom_rtc_exit_config_mode().

	/*---------------------------------------------------------------------------*/
	/** @brief RTC Leave Configuration Mode.

	Revert the RTC to operational state.
	*/

	static void custom_rtc_exit_config_mode(void)
	{
		/* Exit configuration mode. */
		RTC_CRL &= ~RTC_CRL_CNF;

	#ifdef NOTUSED  //  This tends to hang, we skip the wait.
		volatile uint32_t reg32;  ////  Added volatile.
		uint32_t counter = 0;
		/* Wait until the RTOFF bit is 1 (our RTC register write finished). */
		while ((reg32 = (RTC_CRL & RTC_CRL_RTOFF)) == 0) {
			counter++;
			if (counter == 1000) { break; }
		}
	#endif  //  NOTUSED
	}

	/*---------------------------------------------------------------------------*/
	/** @brief RTC Set the Alarm Time.

	@param[in] alarm_time uint32_t. time at which the alarm event is triggered.
	*/

	static void custom_rtc_set_alarm_time(uint32_t alarm_time)
	{
		rtc_enter_config_mode();
		RTC_ALRL = (alarm_time & 0x0000ffff);
		RTC_ALRH = (alarm_time & 0xffff0000) >> 16;
		custom_rtc_exit_config_mode();
	}
#endif  //  NOTUSED
