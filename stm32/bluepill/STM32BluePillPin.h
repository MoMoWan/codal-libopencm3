#ifndef STM32BLUEPILL_PIN_H
#define STM32BLUEPILL_PIN_H

#include "CmPin.h"

namespace codal {
    class STM32BluePillPin : public codal::_cm::Pin {
    public:
        //  Codal constructor
        STM32BluePillPin(
            int id,              //  e.g. DEVICE_ID_IO_P0 + CM_PIN_PC13
            PinName       name,  //  e.g. CM_PIN_PC13
            PinCapability capability  //  e.g. PIN_CAPABILITY_DIGITAL
        ): codal::_cm::Pin(id, name, capability) {}

        //  New constructor
        STM32BluePillPin(
            PinNumber   name,  //  e.g. CM_PIN_PC13
            CmPinRCC    rcc,   //  e.g. RCC_GPIOC
            CmPinPort   port,  //  e.g. GPIOC
            CmPinNumber pin,   //  e.g. GPIO13
            PinCapability capability  //  e.g. PIN_CAPABILITY_DIGITAL
        ): codal::_cm::Pin(name, rcc, port, pin, capability) {}

#ifdef NOTUSED        
    protected:
        void* pin;

    public:
        STM32BluePillPin(int id, PinNumber name, PinCapability capability);
        int getAnalogValue() override;
#endif  //  NOTUSED
    };
}

#endif  //  STM32BLUEPILL_PIN_H
