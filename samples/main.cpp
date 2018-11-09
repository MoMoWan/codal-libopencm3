//  Main Application.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
#include "STM32BluePill.h"

using namespace codal;
STM32BluePill bluepill;

int main()
{
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
