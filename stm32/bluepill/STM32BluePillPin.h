#ifndef STM32BLUEPILL_PIN_H
#define STM32BLUEPILL_PIN_H

#include "CmPin.h"

namespace codal {
    class STM32BluePillPin : public codal::_cm::Pin {
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
