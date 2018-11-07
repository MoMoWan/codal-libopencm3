#ifndef F103RE_PIN_H
#define F103RE_PIN_H

#include "MbedPin.h"
#include "mbed.h"

namespace codal {
    class F103REPin : public codal::_mbed::Pin {
    protected:
        void* pin;

    public:
        int getAnalogValue() override;

        F103REPin(int id, PinNumber name, PinCapability capability);
    };
}

#endif
