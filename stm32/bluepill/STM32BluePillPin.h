#ifndef STM32BLUEPILL_PIN_H
#define STM32BLUEPILL_PIN_H

#include "CmPin.h"

namespace codal {
    class STM32BluePillPin : public codal::_cm::Pin {
    protected:
        void* pin;

    public:
        int getAnalogValue() override;

        STM32BluePillPin(int id, PinNumber name, PinCapability capability);
    };
}

#endif
