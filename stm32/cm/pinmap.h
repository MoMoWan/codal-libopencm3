//  Functions to fetch peripheral (e.g. SPI1), pin mode and config by pin map.
#ifndef CM_PINMAP_H
#define CM_PINMAP_H

#include "platform_includes.h"
#include "PinNamesTypes.h"

// typedef uint32_t PinName;
// #define NC (PinName)(-1)

#ifdef __cplusplus
extern "C"
{
#endif  //  __cplusplus
    typedef struct {
        //  For a peripheral, maps the peripheral function to a pin.
        PinName pin;    //  0 to 127
        uint32_t peripheral; //  e.g. SPI1
        uint8_t remapped;    //  1 if this is a remapped pin.
        uint8_t mode;   //  e.g. GPIO_MODE_OUTPUT_2_MHZ
        uint8_t cnf;    //  e.g. GPIO_CNF_OUTPUT_PUSHPULL
    } PinMap;

    //  Return the peripheral for the pin e.g. SPI1.
    uint32_t pinmap_peripheral(PinName pin, const PinMap *map);

    //  Return the pin mode for the peripheral e.g. GPIO_MODE_OUTPUT_2_MHZ.
    uint8_t pinmap_mode(PinName pin, const PinMap *map);

    //  Return the pin config for the peripheral e.g. GPIO_CNF_OUTPUT_PUSHPULL.
    uint8_t pinmap_cnf(PinName pin, const PinMap *map);

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
