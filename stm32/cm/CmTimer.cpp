#include "CmTimer.h"
#include "CodalCompat.h"
#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "codal_target_hal.h"
#include <bluepill.h>
#include <logger.h>

namespace codal
{
    namespace _cm
    {
        Timer *Timer::instance;

        Timer::Timer() : codal::Timer() {
            instance = this;
            this->prev = 0;
            init();
#ifdef TODO
            memset(&TimHandle, 0, sizeof(TimHandle));
#endif  //  TODO
        }

        extern "C" void TIM5_IRQHandler() {
#ifdef TODO
            auto h = &Timer::instance->TimHandle;

            if (__HAL_TIM_GET_FLAG(h, TIM_FLAG_CC1) == SET)
            {
                if (__HAL_TIM_GET_IT_SOURCE(h, TIM_IT_CC1) == SET)
                {
                    __HAL_TIM_CLEAR_IT(h, TIM_IT_CC1);
                    __HAL_TIM_CLEAR_FLAG(h, TIM_FLAG_CC1);
                    ZTimer::instance->trigger();
                }
            }
#endif  //  TODO
        }

        void tick_callback() {
            //  Will be called at every millisecond tick.  Needed to keep Codal scheduler running.
            //  TODO: Remove this if alarm is working.
            if (!Timer::instance) { return; }  //  No timer to trigger.
            ////Timer::instance->trigger();
        }

        void alarm_callback() {
            //  Will be called when an alarm is triggered.  Needed to keep Codal scheduler running.
            if (!Timer::instance) { return; }  //  No timer to trigger.
            Timer::instance->trigger();
        }

        void Timer::init() {
            debug_println("timer init"); ////
            this->prev = millis();
            target_set_tick_callback(tick_callback);
            target_set_alarm_callback(alarm_callback);
#ifdef TODO
            TimHandle.Instance = TIM5;

            TimHandle.Init.Period = 0xFFFFFFFF;
            TimHandle.Init.Prescaler = (uint32_t)((SystemCoreClock / 1000000) - 1);
            TimHandle.Init.ClockDivision = 0;
            TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

            if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
                CODAL_ASSERT(0);

            NVIC_SetPriority(TIM5_IRQn, 0);
            NVIC_EnableIRQ(TIM5_IRQn);
            HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1);
#endif  //  TODO
        }

        void Timer::triggerIn(CODAL_TIMESTAMP t) {
            //  TODO: Set alarm for millis() + t millisecs.  If alarm is already set and alarm > millis() and alarm < millis() + t, don't set alarm.
            //  debug_print("triggerIn "); debug_println((size_t) t); debug_flush(); ////
            platform_set_alarm(millis() + t);
#ifdef TODO
            if (t < 20)
                t = 20;

            this->syncRequest(); // is this needed?

            target_disable_irq();
            __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_CC1);
            __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1,
                                (uint32_t)(__HAL_TIM_GET_COUNTER(&TimHandle) + t));
            __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_CC1);
            target_enable_irq();
#endif  //  TODO
        }

        // extern "C" uint32_t uwTick;  //  Elapsed milliseconds.

        void Timer::syncRequest() {
            target_disable_irq();
            uint32_t curr = millis();
            uint32_t delta = curr - this->prev;  //  In milliseconds.

            // update the hal...
            // uwTick += delta;
            this->prev = curr;
            this->sync(delta * 1000);  //  Sync expects microseconds.
            target_enable_irq();            
            //  debug_print("timer sync "); debug_println((size_t) delta * 1000);
        }

        extern "C" void wait_us(uint32_t us) {
            target_wait_us(us);
        }
    }  //  namespace _cm

} // namespace codal
