#include "STM32BluePillPin.h"


namespace codal {
    STM32BluePillPin::STM32BluePillPin(int id, PinNumber name, PinCapability capability) : 
    codal::_cm::Pin(id, name, capability) {
        this->pullMode = DEVICE_DEFAULT_PULLMODE;
        this->status = 0x00;
        this->pin = NULL;
    }

    int STM32BluePillPin::getAnalogValue() {
        if (!(PIN_CAPABILITY_ANALOG & capability))
            return DEVICE_NOT_SUPPORTED;

        return 225; ////TODO
#ifdef TODO
        if (!(status & IO_STATUS_ANALOG_IN)) {
            disconnect();
            pin = new AnalogIn((PinName)name);
            status |= IO_STATUS_ANALOG_IN;
        }

        return ((AnalogIn*)pin)->read_u16() >> 6;
#endif  //  TODO
    }
}
