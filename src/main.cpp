//  Main Application for PlatformIO build.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
//  TODO: Sync with samples/main.cpp
#include <newlib-force.h>  //  Force newlib to be included for build
#include <qfplib.h>        //  Force qfplib to be included for build
#include <nano-float.h>    //  Force nano-float to be included for build
#include "STM32BluePill.h"
#include <logger.h>

#ifdef PLATFORMIO  //  If building on PlatformIO...
#include "i2cint.h"  //  Force I2C Interface to be included for PlatformIO build.
#include "spiint.h"  //  Force SPI Interface to be included for PlatformIO build.
#include "adcint.h"  //  Force ADC Interface to be included for PlatformIO build.
#endif  //  PLATFORMIO

using namespace codal;

void Blink_main(codal::STM32BluePill& bluepill);

int main() {
    //  Note: Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    target_init();               //  Init the STM32 platform.

    //  Blue Pill constructor will generate debug messages, so we construct after enabling debug.
    STM32BluePill bluepill;
    bluepill.init();
    Blink_main(bluepill);
}

void debug_dump() {
    debug_print("t "); debug_print((size_t) millis()); 
    debug_print(", alarm "); debug_print((size_t) platform_alarm_count()); 
    debug_println(""); debug_flush();
}

void Blink_main(codal::STM32BluePill& bluepill) {
    debug_dump();
	bluepill.io.led.setDigitalValue(0);

	int state = 1;
    int counter = 0;
	while(1) {    
        //  Blink the LED and pause 1 second.
		bluepill.io.led.setDigitalValue(state);
        bluepill.sleep(1000);
        state = !state;
        debug_dump();

        //  Test Deep Sleep Standby Mode.
        counter++;
        //  At t = 5 seconds, set the wakeup alarm for t = 30 seconds.
        //  if (counter == 5) { platform_set_alarm(30 * 1000); }

        //  At t = 20 seconds, enter deep sleep standby mode.  Don't enter deep sleep too soon, because Blue Pill will not allow reflashing while sleeping.
        //  if (counter == 20) { target_enter_deep_sleep_standby_mode(); }

        //  At t = 30 seconds, device should wakeup by alarm and restart as though t = 0.
    }
}

#ifdef NOTUSED
//  From /framework-libopencm3/lib/cm3/vector.c
#include <libopencm3/cm3/scb.h>
/* Symbols exported by the linker script(s): */
extern unsigned _data_loadaddr, _edata, _ebss;
typedef void (*funcp_t) (void);
extern funcp_t __preinit_array_start, __preinit_array_end;
extern funcp_t __init_array_start, __init_array_end;
extern funcp_t __fini_array_start, __fini_array_end;
void custom_reset_handler() {
    volatile unsigned *src, *dest;
	funcp_t *fp;

	for (src = &_data_loadaddr, dest = (volatile unsigned *) &_data;
		dest < &_edata;
		src++, dest++) {
		*dest = *src;
	}

	while (dest < &_ebss) {
		*dest++ = 0;
	}

	/* Ensure 8-byte alignment of stack pointer on interrupts */
	/* Enabled by default on most Cortex-M parts, but not M3 r1 */
	SCB_CCR |= SCB_CCR_STKALIGN;

	/* might be provided by platform specific vector.c */
	// pre_main();

	/* Constructors. */
	for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__init_array_start; fp < &__init_array_end; fp++) {
		(*fp)();
	}
}

#endif  //  NOTUSED