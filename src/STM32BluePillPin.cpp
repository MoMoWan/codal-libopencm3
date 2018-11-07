#include "F103REPin.h"


namespace codal {
    F103REPin::F103REPin(int id, PinNumber name, PinCapability capability) : codal::_mbed::Pin(id, name, capability) {
        this->pullMode = DEVICE_DEFAULT_PULLMODE;
        this->status = 0x00;
        this->pin = NULL;
    }

    int F103REPin::getAnalogValue() {
        if (!(PIN_CAPABILITY_ANALOG & capability))
            return DEVICE_NOT_SUPPORTED;

        if (!(status & IO_STATUS_ANALOG_IN)) {
            disconnect();
            pin = new AnalogIn((PinName)name);
            status |= IO_STATUS_ANALOG_IN;
        }

        return ((AnalogIn*)pin)->read_u16() >> 6;
    }
}
