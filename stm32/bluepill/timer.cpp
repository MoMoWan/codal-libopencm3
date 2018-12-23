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

//  Select Oscillator for the realtime clock: 
//  RCC_HSE: 62.5 kHz, fastest oscillator, doesn't work in Stop or Standby Low Power mode. 
//  RCC_LSE: 32.768 kHz, slowest oscillator, works in Stop or Standby Low Power mode. 
//  RCC_LSI: 40 kHz, works in Stop or Standby Low Power mode. 
//  We choose RCC_LSE because it can be used to wake up in Low Power mode.
	
#ifdef LIBOPENCM3_RCC_LEGACY  //  If using older version of libopencm3 (PlatformIO)...
const rcc_osc clock_source = LSE;         //  Oscillator is named LSE.
//  const rcc_osc clock_source = HSE;     //  Oscillator is named HSE.
#else                         //  If using newer version of libopencm3 (Codal)...
const rcc_osc clock_source = RCC_LSE;     //  Oscillator is named RCC_LSE.
//  const rcc_osc clock_source = RCC_HSE; //  Oscillator is named RCC_HSE.
#endif  //  LIBOPENCM3_RCC_LEGACY

//  Set the clock prescaling value, so that we will get a tick interrupt every 1 millisecond. Dependent on LSE or HSE clock selection.
const uint32_t prescale = 32;        //  For RCC_LSE: 1 millisecond tick (should actually be 32.7)
// const uint32_t prescale = 62;     //  For RCC_HSE: 1 millisecond tick (should actually be 62.5)
// const uint32_t prescale = 62500;  //  For RCC_HSE: 1 second tick

//  This is the tick function we will call every millisecond.  
//  Usually points to os_tick() in cocoOS.
static void (*tickFunc)(void) = NULL;
static volatile uint32_t tickCount = 0;  //  Number of millisecond ticks elapsed.
static volatile uint32_t alarmCount = 0;  //  Number of alarms elapsed.

static void rtc_setup(void) {
	//  Setup RTC interrupts for tick and alarm wakeup.
	debug_println("rtc awake..."); debug_flush(); //  rtc_awake_from_off() fails on qemu.

	//  From: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32vl-discovery/rtc/rtc.c
	//  rtc_auto_awake(): If the RTC is pre-configured just allow access, don't reconfigure.
	//  Otherwise enable it with the clock source and set the prescale value.
	//  rtc_auto_awake(clock_source, prescale);

	//  rtc_auto_awake() will not reset the RTC when you press the RST button.
	//  It will also continue to count while the MCU is held in reset. If
	//  you want it to reset, comment out the above and use the following:
	rtc_awake_from_off(clock_source);  //  This will enable RTC.
	rtc_set_prescale_val(prescale);
	debug_println("rtc awake ok"); debug_flush(); //  rtc_awake_from_off() fails on qemu.

	//  RTC should already be enabled in rtc_awake_from_off().
	rcc_enable_rtc_clock();
	rtc_interrupt_disable(RTC_SEC);
	rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_disable(RTC_OW);
	
	rtc_set_counter_val(1);  //  Start counting millisecond ticks from 1 so we won't trigger alarm.
	rtc_set_alarm_time(0);   //  Reset alarm to 0.
	exti_set_trigger(EXTI17, EXTI_TRIGGER_RISING);  //  Enable alarm wakeup.
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

void platform_start_timer(void (*tickFunc0)(void)) {
    //  Start the STM32 Timer to generate interrupt ticks to perform task switching.
  	tickFunc = tickFunc0;  //  Allow tickFunc to be modified at every call to platform_start_timer().
	
	//  But system timer will only be started once.
	static bool timerStarted = false;
	if (timerStarted) { return; }
	timerStarted = true;
	rtc_setup();
}

void platform_set_alarm(uint32_t millisec) {
	//  Set alarm for millisec milliseconds elapsed since startup.
	rtc_set_alarm_time(millisec);
	//  TODO: rtc_enable_alarm()
}

uint32_t platform_get_alarm(void) {
	//  Get alarm time.
	return rtc_get_alarm_val();
}

void rtc_isr(void) {
	//  Interrupt Service Routine for RTC Tick, Alarm, Overflow.  Don't call any I/O functions here.
	if (rtc_check_flag(RTC_SEC)) {
		//  We hit an RTC tick interrupt.
		rtc_clear_flag(RTC_SEC);
		tickCount++;
		//  Call the tick function os_tick() to perform multitasking.
		if (tickFunc != NULL) { tickFunc(); }
		return;
	}
	if (rtc_check_flag(RTC_ALR)) {
		//  We hit an RTC alarm interrupt.
		rtc_clear_flag(RTC_ALR);
		return;
	}
}

void rtc_alarm_isr(void) {
	//  Interrupt Service Routine for RTC Alarm Wakeup.  Don't call any I/O functions here.
	alarmCount++;
	exti_reset_request(EXTI17);
	rtc_clear_flag(RTC_ALR);
}

uint32_t millis(void) {
	//  Return the number of millisecond ticks since startup.
	//  Compatible with Arduino's millis() function.
	//  TODO: Compensate for clock slowdown because we truncated RCC_LSE 32.768 kHz to 32.
	return rtc_get_counter_val();  //  More accurate, uses hardware counters.
	// return tickCount;  //  Less accurate, excludes ARM Semihosting time. 
}

uint32_t platform_alarm_count(void) {
	//  Return the number of alarms triggered since startup.
	return alarmCount;
}
