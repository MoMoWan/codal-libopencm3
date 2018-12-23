//  Main Application.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
//  TODO: Sync with src/main.cpp

#include "STM32BluePill.h"
using namespace codal;

void Blink_main(codal::STM32BluePill& bluepill);

int main()
{
    //  Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();   //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.

    //  Blue Pill constructor will generate debug messages, so we construct after enabling debug.
    STM32BluePill bluepill;
    bluepill.init();
    Blink_main(bluepill);
}

void Blink_main(codal::STM32BluePill& bluepill){
	bluepill.io.led.setDigitalValue(0);

	int state = 1;
	while(1)
    {    
		bluepill.io.led.setDigitalValue(state);
        bluepill.sleep(1000);
        state = !state;
    }
}
