//  Based on https://github.com/nedseb/codal-stm32-iot-node/blob/master/model/STM32IotNode.h
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

#ifndef STM32BLUEPILL_H
#define STM32BLUEPILL_H

#include "CodalHeapAllocator.h"
#include "Event.h"
#include "CodalDevice.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalComponent.h"
#include "CodalDmesg.h"
#include "ManagedType.h"
#include "ManagedString.h"
#include "NotifyEvents.h"

#include "Button.h"
#include "MultiButton.h"
#include "STM32BluePillIO.h"
#include "CodalFiber.h"
#include "MessageBus.h"
#include "CmTimer.h"
#include "CmI2C.h"
#include "bluepill.h"

// Status flag values
#define DEVICE_INITIALIZED                    0x01

/**
 * Class definition for a STM32BluePill device.
 *
 * Represents the device as a whole, and includes member variables that represent various device drivers
 * used to control aspects of the micro:bit.
 */
namespace codal
{
    class STM32BluePill : public CodalComponent
    {
        public:
            codal::_cm::Timer         timer;
            MessageBus                messageBus;
            STM32BluePillIO           io;
            codal::_cm::I2C           i2c;

            /**
             * Constructor.
             */
            STM32BluePill();
            void enableDebug();
            void disableDebug();

            /**
             * Post constructor initialisation method.
             */
            int init();

            /**
             * Delay execution for the given amount of time.
             *
             * If the scheduler is running, this will deschedule the current fiber and perform
             * a power efficient, concurrent sleep operation.
             *
             * If the scheduler is disabled or we're running in an interrupt context, this
             * will revert to a busy wait.
             *
             * Alternatively: wait, wait_ms, wait_us can be used which will perform a blocking sleep
             * operation.
             *
             * @param milliseconds the amount of time, in ms, to wait for. This number cannot be negative.
             *
             */
            void sleep(uint32_t milliseconds);

            /**
             * A periodic callback invoked by the fiber scheduler idle thread.
             * We use this for any low priority, background housekeeping.
             */
            virtual void idleCallback();

            /**
             * A periodic callback invoked by the fiber scheduler every SCHEDULER_TICK_PERIOD_MS.
             */
            virtual void periodicCallback();
            
            /**
             * Determine the time since this STM32BLUEPILL was last reset.
             *
             * @return The time since the last reset, in milliseconds.
             *
             * @note This will value overflow after 1.6 months.
             */
            //TODO: handle overflow case.
            unsigned long systemTime();
    };


    /**
     * Delay execution for the given amount of time.
     *
     * If the scheduler is running, this will deschedule the current fiber and perform
     * a power efficient, concurrent sleep operation.
     *
     * If the scheduler is disabled or we're running in an interrupt context, this
     * will revert to a busy wait.
     *
     * Alternatively: wait, wait_ms, wait_us can be used which will perform a blocking sleep
     * operation.
     *
     * @param milliseconds the amount of time, in ms, to wait for. This number cannot be negative.
     *
     * @return STM32BLUEPILL_OK on success, STM32BLUEPILL_INVALID_PARAMETER milliseconds is less than zero.
     *
     */
    inline void STM32BluePill::sleep(uint32_t milliseconds)
    {
        fiber_sleep(milliseconds);
    }

    /**
     * Determine the time since this STM32BLUEPILL was last reset.
     *
     * @return The time since the last reset, in milliseconds.
     *
     * @note This will value overflow after 1.6 months.
     */
    inline unsigned long STM32BluePill::systemTime()
    {
        return system_timer_current_time();
    }
}

using namespace codal;

#endif
