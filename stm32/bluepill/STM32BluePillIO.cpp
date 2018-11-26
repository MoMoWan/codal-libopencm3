//  Based on https://github.com/nedseb/codal-stm32-iot-node/blob/master/model/STM32IotNodeIO.cpp
/*
  The MIT License (MIT)

  Copyright (c) 2017 Lancaster University.

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/

/**
  * Class definition for MicroBit NEXT IO.
  * Represents a collection of all I/O pins on the device.
  */

#include "CodalConfig.h"
#include "STM32BluePillIO.h"

using namespace codal;

/**
  * Constructor.
  *
  * Create a representation of all given I/O pins on the edge connector
  *
  * Accepts a sequence of unique ID's used to distinguish events raised
  * by MicroBitPin instances on the default EventModel.
  */
STM32BluePillIO::STM32BluePillIO() :
    scl (ID_PIN_SCL, PB_6, PIN_CAPABILITY_AD),
    sda (ID_PIN_SDA, PB_7, PIN_CAPABILITY_AD),
    led (ID_PIN_LED_GREEN, PC_13, PIN_CAPABILITY_AD),
    pc13 (ID_PIN_P13, PC_13, PIN_CAPABILITY_AD),
    temperature (ID_PIN_TEMP_SENSOR, ADC_TEMP, PIN_CAPABILITY_ANALOG)
{
}
