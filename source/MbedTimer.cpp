#include "MbedTimer.h"
#include "CodalCompat.h"
#include "Timer.h"
#include "CodalConfig.h"
#include "codal_target_hal.h"

#include "CodalDmesg.h"
#define MINIMUM_PERIOD  32

namespace codal
{
    namespace mbed
    {

        void Timer::triggered()
        {
            timeout.attach_us(this, &Timer::triggered, this->period);
            this->trigger();
        }

        Timer::Timer() : codal::Timer()
        {
            this->period = 10000;
            timer.start();
            timeout.attach_us(this, &Timer::triggered, this->period);
        }

        /**
         * request to the physical timer implementation code to provide a trigger callback at the given time.
         * note: it is perfectly legitimate for the implementation to trigger before this time if convenient.
         * @param t Indication that t time units (typically microsends) have elapsed.
         */
        void Timer::triggerIn(CODAL_TIMESTAMP t)
        {
            if (t < MINIMUM_PERIOD)
                t = MINIMUM_PERIOD;

            timeout.detach();
            timeout.attach_us(this, &Timer::triggered, t);
        }
        /**
         * request to the physical timer implementation code to trigger immediately.
         */
        void Timer::syncRequest()
        {
            target_disable_irq();
            int elapsed = timer.read_us();
            target_enable_irq();
            timer.reset();
            this->sync(elapsed);
        }
    }
}