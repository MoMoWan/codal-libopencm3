#include "MbedTimer.h"
#include "Event.h"
#include "CodalCompat.h"
#include "Timer.h"

namespace codal
{
    namespace mbed
    {
        static uint64_t current_time_us = 0;
        static uint32_t overflow_period_us = 0;

        static TimerListComparator tlc;
        static List eventList = List(tlc);

        void Timer::processEvents()
        {
            START:

            if(eventList.length() == 0)
                return;

            TimerEvent* tmp = NULL;

            for (int i = 0; i < eventList.length(); i++)
            {
                tmp = (TimerEvent *)eventList.get(i).getBytes();

                if(tmp->timestamp > getTimeUs())
                    break;

                // fire our event and process the next event
                Event(tmp->id, tmp->value);

                eventList.remove(i);

                // if this event is non-repeating, delete
                if(tmp->period > 0)
                {
                    // update our count, and readd to our event list
                    tmp->timestamp += tmp->period;
                    eventList.insert((uint8_t *)tmp, sizeof(TimerEvent));
                }
            }

            // what about after we removed from the list? We will have zero events, therefore the following code is void.

            // Take the new head of the list (which may have changed)
            // as this will be the event that is due next.
            tmp = (TimerEvent *)eventList.get(0).getBytes();

            uint64_t now = getTimeUs();

            uint64_t usRemaining = tmp->timestamp - now;

            if (tmp->timestamp < now || usRemaining < 100)
                goto START;

            if(usRemaining < overflow_period_us)
                timeout.attach_us(this, &Timer::processEvents, usRemaining);
        }

        void Timer::timerOverflow()
        {
            timer.reset();
            overflowTimeout.attach_us(this, &Timer::timerOverflow, overflow_period_us);

            current_time_us += overflow_period_us;

            processEvents();
        }

        /**
          * Constructor for an instance of Timer1.
          *
          * @param id The id to use for the message bus when transmitting events.
          */
        Timer::Timer(uint16_t id) : timer(), timeout(), overflowTimeout()
        {
            if(system_timer_get_instance() == NULL)
                system_timer_set_instance(this);

            overflow_period_us = 60000000;

            this->id = id;
        }

        /**
          * Returns the id for this timer instance
          */
        int Timer::getId()
        {
            return this->id;
        }

        /**
          * Initialises and starts this Timer instance
          */
        int Timer::init()
        {
            if(status & SYSTEM_CLOCK_INIT)
                return DEVICE_OK;

            timer.start();

            overflowTimeout.attach_us(this, &Timer::timerOverflow, overflow_period_us);

            status |= SYSTEM_CLOCK_INIT;

            return DEVICE_OK;
        }

        /**
          * Sets the current time tracked by this Timer instance
          * in milliseconds
          *
          * @param timestamp the new time for this Timer instance in milliseconds
          */
        int Timer::setTime(uint64_t timestamp)
        {
            return setTimeUs(timestamp * 1000);
        }

        /**
          * Sets the current time tracked by this Timer instance
          * in microseconds
          *
          * @param timestamp the new time for this Timer instance in microseconds
          */
        int Timer::setTimeUs(uint64_t timestamp)
        {
            current_time_us = timestamp;
            return DEVICE_OK;
        }

        /**
          * Retrieves the current time tracked by this Timer instance
          * in milliseconds
          *
          * @return the timestamp in milliseconds
          */
        uint64_t Timer::getTime()
        {
            return getTimeUs() / 1000;
        }

        /**
          * Retrieves the current time tracked by this Timer instance
          * in microseconds
          *
          * @return the timestamp in microseconds
          */
        uint64_t Timer::getTimeUs()
        {
            return current_time_us + timer.read_us();
        }

        int Timer::configureEvent(uint64_t period, uint16_t id, uint16_t value, bool repeating)
        {
            TimerEvent t(getTimeUs() + period, period, id, value, repeating);

            __disable_irq();
            eventList.insert((uint8_t *)&t, sizeof(TimerEvent));
            __enable_irq();

            if (eventList.length() == 1 && period < overflow_period_us)
                timeout.attach_us(this, &Timer::processEvents, period);

            return DEVICE_OK;
        }

        /**
          * Configures this Timer instance to fire an event after period
          * milliseconds.
          *
          * @param period the period to wait until an event is triggered, in milliseconds.
          *
          * @param value the value to place into the Events' value field.
          */
        int Timer::eventAfter(uint64_t interval, uint16_t id, uint16_t value)
        {
            return eventAfterUs(interval * 1000, id, value);
        }

        /**
          * Configures this Timer instance to fire an event after period
          * microseconds.
          *
          * @param period the period to wait until an event is triggered, in microseconds.
          *
          * @param value the value to place into the Events' value field.
          */
        int Timer::eventAfterUs(uint64_t interval, uint16_t id, uint16_t value)
        {
            return configureEvent(interval, id, value, false);
        }

        /**
          * Configures this Timer instance to fire an event every period
          * milliseconds.
          *
          * @param period the period to wait until an event is triggered, in milliseconds.
          *
          * @param value the value to place into the Events' value field.
          */
        int Timer::eventEvery(uint64_t period, uint16_t id, uint16_t value)
        {
            return eventEveryUs(period * 1000, id, value);
        }

        /**
          * Configures this Timer instance to fire an event every period
          * microseconds.
          *
          * @param period the period to wait until an event is triggered, in microseconds.
          *
          * @param value the value to place into the Events' value field.
          */
        int Timer::eventEveryUs(uint64_t period, uint16_t id, uint16_t value)
        {
            return configureEvent(period, id, value, true);
        }

        /**
          * Cancels any events matching the given id and value.
          *
          * @param id the ID that was given upon a previous call to eventEvery / eventAfter
          *
          * @param value the value that was given upon a previous call to eventEvery / eventAfter
          */
        int Timer::cancel(uint16_t id, uint16_t value)
        {
            TimerEvent t(id, value);

            __disable_irq();
            eventList.remove((uint8_t*) &t, sizeof(TimerEvent));
            __enable_irq();

            return DEVICE_OK;
        }
    }
}
