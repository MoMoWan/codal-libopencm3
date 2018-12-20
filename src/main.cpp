//  Main Application.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
#include <newlib-force.h>  //  Force newlib to be included for build
#include "STM32BluePill.h"
#include <logger.h>

#ifdef PLATFORMIO  //  If building on PlatformIO...
#include "i2cint.h"  //  Force I2C Interface to be included for PlatformIO build.
#include "spiint.h"  //  Force SPI Interface to be included for PlatformIO build.
#include "adcint.h"  //  Force ADC Interface to be included for PlatformIO build.
//  Handle exit.  From https://arobenko.gitbooks.io/bare_metal_cpp/content/compiler_output/static.html.
extern "C" {
    void* __dso_handle = nullptr;
    void _fini(void) { }
    int __wrap_atexit( 
        void *object, 
        void (*destructor)(void *), 
        void *dso_handle) 
    { 
        static_cast<void>(object); 
        static_cast<void>(destructor); 
        static_cast<void>(dso_handle); 
        return 0; 
    }
}
#endif  //  PLATFORMIO

using namespace codal;

void Blink_main(codal::STM32BluePill& bluepill);
STM32BluePill bluepill;

int main() {
    //  Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    //  bluepill.enableDebug();   //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no debugger is attached.
    bluepill.disableDebug();  //  Uncomment to disable display of debug messages.  For use in production devices.
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
        if (counter == 5) { platform_set_alarm(30 * 1000); }
        //  At t = 20 seconds, enter deep sleep standby mode.  Don't enter deep sleep too soon, because Blue Pill will not allow reflashing while sleeping.
        if (counter == 20) { target_enter_deep_sleep_standby_mode(); }
        //  At t = 30 seconds, device should wakeup by alarm and restart as though t = 0.
    }
}
