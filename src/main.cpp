//  Main Application for PlatformIO build.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
//  TODO: Sync with samples/main.cpp
#include <logger.h>
#include <STM32BluePill.h>

#ifdef PLATFORMIO    //  If building on PlatformIO...
#include "i2cint.h"  //  Force I2C Interface to be included for PlatformIO build.
#include "spiint.h"  //  Force SPI Interface to be included for PlatformIO build.
#include "adcint.h"  //  Force ADC Interface to be included for PlatformIO build.
#include "unity.h"   //  Force Unity unit test to be included for PlatformIO build.
#endif  //  PLATFORMIO

using namespace codal;

void Blink_main(codal::STM32BluePill& bluepill);
static void debug_dump(codal::STM32BluePill& bluepill, const char *msg);

int main() {
    //  This main() function will be wrapped by the linker.  The actual main() function is in stm32/hal/main.cpp.
    //  Blue Pill constructor will generate debug messages, so we construct after enabling debug.
    STM32BluePill bluepill;
    bluepill.init();
    Blink_main(bluepill);
}

void Blink_main(codal::STM32BluePill& bluepill) {
    debug_dump(bluepill, "set led");
	bluepill.io.led.setDigitalValue(0);

    debug_dump(bluepill, "target wait");
    target_wait_us(1000000);  //  Test the RTC.

    debug_dump(bluepill, "sleep");
    bluepill.sleep(1000);     //  Test the CODAL scheduler.

	int state = 1;
    int counter = 0;
    debug_dump(bluepill, "loop");
	while(1) {    
        //  Blink the LED and pause 1 second.
		bluepill.io.led.setDigitalValue(state);
        debug_dump(bluepill, "sleep");
        bluepill.sleep(1000);
        state = !state;

        //  Test Deep Sleep Standby Mode.
        counter++;
        //  At t = 5 seconds, set the wakeup alarm for t = 30 seconds.
        //  if (counter == 5) { platform_set_alarm(30 * 1000); }

        //  At t = 20 seconds, enter deep sleep standby mode.  Don't enter deep sleep too soon, because Blue Pill will not allow reflashing while sleeping.
        //  if (counter == 20) { target_enter_deep_sleep_standby_mode(); }

        //  At t = 30 seconds, device should wakeup by alarm and restart as though t = 0.
    }
}

static void debug_dump(codal::STM32BluePill& bluepill, const char *msg) {
    debug_print(msg);
    debug_print(" t "); debug_print((size_t) millis()); 
    debug_print(", ms "); debug_print((size_t) bluepill.timer.getTime()); 
    debug_print(", us "); debug_print((size_t) bluepill.timer.getTimeUs()); 
    debug_print(", alarm "); debug_print((size_t) platform_alarm_count()); 
    debug_print(", tick "); debug_print((size_t) platform_tick_count()); 
    debug_println(""); debug_flush();
}
