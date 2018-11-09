//  Main Application.  Based on https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node/blob/master/samples/main.cpp
#include "stm32.h"
#include "STM32BluePill.h"

int main2(void);

//  using namespace codal;
codal::STM32BluePill bluepill;

int main()
{
    bluepill.init();
    #if defined(BLE_TEMPERATURE_ALARM_SAMPLE)
    ////TemperatureAlarm_main(iotNode);
    #else
    main2();
    #endif
}

//  Rename the old main() function as main2()
#define main main2
#include "../src/main.cpp"
