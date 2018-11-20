//  Main Application.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
#include "STM32BluePill.h"
#include <logger.h>

#ifdef PLATFORMIO  //  If building on PlatformIO...
#include "i2cint.h"  //  Force I2C Interface to be included for PlatformIO build.
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

int main()
{
    bluepill.init();
    Blink_main(bluepill);
}

void Blink_main(codal::STM32BluePill& bluepill){
    // debug_println((size_t) codal::system_timer_current_time()); debug_flush(); ////
	bluepill.io.led.setDigitalValue(0);

	int state = 1;
	while(1)
    {    
        // debug_println((size_t) codal::system_timer_current_time()); debug_flush(); ////
		bluepill.io.led.setDigitalValue(state);
        // debug_println((size_t) codal::system_timer_current_time()); debug_flush(); ////
        bluepill.sleep(1000);
        // debug_println((size_t) codal::system_timer_current_time()); debug_flush(); ////
        state = !state;
    }
}
