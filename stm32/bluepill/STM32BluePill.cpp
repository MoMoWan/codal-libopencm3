//  Based on https://github.com/nedseb/codal-stm32-iot-node/blob/master/model/STM32IotNode.cpp
/*
    The MIT License (MIT)

    Copyright (c) 2016 Lancaster University, UK.

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


#include "STM32BluePill.h"
#include "Timer.h"

using namespace codal;

static STM32BluePill *device_instance = NULL;

/**
  * Constructor.
  *
  * Create a representation of a GenuinoZero device, which includes member variables
  * that represent various device drivers used to control aspects of the micro:bit.
  */
STM32BluePill::STM32BluePill() :
    timer(),
    messageBus(),
    io(),
    i2c(io.sda, io.scl),
    spi(io.mosi, io.miso, io.sck),
    serial(io.tx, io.rx) {  //  Note: Default serial port is USART2 (TX2, RX2).
    //  Clear our status
    status = 0;
    device_instance = this;
}

void STM32BluePill::enableDebug() { target_enable_debug(); }
void STM32BluePill::disableDebug() { target_disable_debug(); }

/**
  * Post constructor initialisation method.
  *
  * This call will initialised the scheduler, memory allocator and Bluetooth stack.
  *
  * This is required as the Bluetooth stack can't be brought up in a
  * static context i.e. in a constructor.
  *
  * @code
  * uBit.init();
  * @endcode
  *
  * @note This method must be called before user code utilises any functionality
  *       contained within the GenuinoZero class.
  */
int STM32BluePill::init()
{
    if (status & DEVICE_INITIALIZED)
        return DEVICE_NOT_SUPPORTED;
    status |= DEVICE_INITIALIZED;

    //  Blue Pill specific initialisation...
    target_init();

    //  Codal initialisation...
    //  Bring up fiber scheduler.
    scheduler_init(messageBus);
    for(int i = 0; i < DEVICE_COMPONENT_COUNT; i++)
    {
        if(CodalComponent::components[i])
            CodalComponent::components[i]->init();
    }
    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;
    return DEVICE_OK;
}

/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  * We use this for any low priority, backgrounf housekeeping.
  *
  */
void STM32BluePill::idleCallback() {
    codal_dmesg_flush();
}

void STM32BluePill::periodicCallback() {
    //  TODO
}

