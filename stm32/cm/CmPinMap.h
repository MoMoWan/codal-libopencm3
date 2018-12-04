//  Functions to fetch peripheral (e.g. SPI1), pin mode and config by pin map.
#ifndef CM_PINMAP_H
#define CM_PINMAP_H

#include "platform_includes.h"
#include "CmPin.h"      //  For CmPeripheral, CmPinMode, CmPinCnf
#include "CmPinName.h"  //  For PinName
// #include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  //  __cplusplus

    typedef struct {
        //  For a peripheral and pin, this record defines the pin mode and config to be used.
        PinName pin;      //  e.g. CM_PIN_SPI1
        CmPeripheral peripheral; //  e.g. SPI1
        uint8_t remapped; //  1 if this is a remapped pin.
        CmPinMode mode;   //  e.g. GPIO_MODE_OUTPUT_2_MHZ
        CmPinCnf cnf;     //  e.g. GPIO_CNF_OUTPUT_PUSHPULL
    } PinMap;

    //  Return the peripheral for the pin e.g. SPI1.
    CmPeripheral pinmap_peripheral(PinName pin, const PinMap *map);

    //  Return the pin mode for the peripheral e.g. GPIO_MODE_OUTPUT_2_MHZ.
    CmPinMode pinmap_mode(PinName pin, const PinMap *map);

    //  Return the pin config for the peripheral e.g. GPIO_CNF_OUTPUT_PUSHPULL.
    CmPinCnf pinmap_cnf(PinName pin, const PinMap *map);

#ifdef __cplusplus
}
#endif  //  __cplusplus

#ifdef TODO
    #ifdef __cplusplus
        namespace codal {
            class Pin;
            uint32_t codal_setup_pin(Pin *p, uint32_t prev, const PinMap *map);
        } // namespace codal
    #endif   //  __cplusplus
#endif  //  TODO

#endif  //  CM_PINMAP_H
