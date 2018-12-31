#ifndef CM_TIMER_H
#define CM_TIMER_H

#include <Event.h>
#include <Timer.h>

namespace codal
{
    namespace _cm
    {
        class Timer : public codal::Timer
        {
            uint32_t prev;
            bool initialised;
        public:
            Timer();
            static Timer *instance;
            void init();
            virtual void triggerIn(CODAL_TIMESTAMP t);
            virtual void syncRequest();
        };
    }  //  namespace _cm
} // namespace codal

#endif  //  CM_TIMER_H
