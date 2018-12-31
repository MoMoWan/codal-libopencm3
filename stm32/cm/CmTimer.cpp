#include "CmTimer.h"
#include "CodalCompat.h"
#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "codal_target_hal.h"
#include <bluepill.h>
#include <logger.h>
#include <cocoos.h>

namespace codal
{
    namespace _cm
    {
        Timer *Timer::instance;
        static CODAL_TIMESTAMP trigger_period;

        ////
        static struct TimerContext {
        } context;
        static struct TimerMsg {
            Msg_t super;                          //  Required for all cocoOS messages.
        } msg;
        #define MSG_POOL_SIZE 2
        static TimerMsg msg_pool[MSG_POOL_SIZE];  //  Pool of sensor data messages for the task message queue.
        static Sem_t timer_semaphore;
        static void timer_task(void);
        ////

        Timer::Timer() : codal::Timer() {
            initialised = false;
            instance = this;
            prev = 0;
            trigger_period = 0;
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
            debug_print("T");
            sem_ISR_signal(timer_semaphore); 

            //  Handle tick.
            ////if (!Timer::instance) { return; }  //  No timer to trigger.
            ////Timer::instance->trigger();
        }

        void alarm_callback() {
            //  Will be called when an alarm is triggered.  Needed to keep Codal scheduler running.
            debug_print("ALM ");
            sem_ISR_signal(timer_semaphore); 
            ////if (!Timer::instance) { return; }  //  No timer to trigger.
            ////Timer::instance->trigger();
        }

        void Timer::init() {
            if (initialised) { return; }  //  If already initialised, quit.
            debug_println("timer init"); ////
            initialised = true;
            prev = millis();

            ////
            timer_semaphore = sem_bin_create(0);  //  Binary Semaphore: Will wait until signalled.
            uint8_t task_id = task_create(
                timer_task,   //  Task will run this function.
                &context,  //  task_get_data() will be set to the display object.
                20,             //  Priority 20 = lower priority than UART task
                (Msg_t *) msg_pool,  //  Pool to be used for storing the queue of messages.
                MSG_POOL_SIZE,     //  Size of queue pool.
                sizeof(TimerMsg));   //  Size of queue message.
            ////

            target_set_tick_callback(tick_callback);
            target_set_alarm_callback(alarm_callback);

            //  If we were asked to set a trigger before init(), set it now.
            if (trigger_period > 0) {
                debug_print("init calling triggerIn... ");
                triggerIn(trigger_period);
                trigger_period = 0;
            }

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
            //  Set alarm for millis() + t millisecs.
            if (!initialised) {
                //  If we are called before init(), remember the trigger and set during init().
                trigger_period = t;
                debug_println("triggerIn called before init");
                return;
            }
            //  debug_print("triggerIn "); debug_println((size_t) t); debug_flush(); ////
            debug_printhex_unsigned(platform_get_alarm()); debug_print(" "); debug_flush(); ////
            //  trigger_period = millis() + t;  //  We will set the timer to be triggered at this period when the alarm is raised.
            platform_set_alarm(t);
            debug_printhex_unsigned(platform_get_alarm()); debug_print(" "); debug_flush(); ////
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
            if (!initialised) {
                debug_println("syncRequest called before init");
                return;
            }
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

        ////
        static void timer_task(void) {
            task_open();  //  Start of the task. Must be matched with task_close().  
            for (;;) {
                // msg_receive(os_get_running_tid(), &msg);
                sem_wait(timer_semaphore);
                if (!Timer::instance) { continue; }  //  No timer to trigger.
                debug_print(" >> "); ////
                Timer::instance->trigger();
                debug_flush(); ////
            }
            task_close();  //  End of the task. Should not come here.
        }
        ////

    }  //  namespace _cm

} // namespace codal
