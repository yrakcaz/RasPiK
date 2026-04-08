#include "timers.h"

s_timerarm *timerarm;
s_timersys *timersys;

void init_timers(void)
{
    timerarm = (s_timerarm *)BASE_TIMERARM;
    timersys = (s_timersys *)BASE_TIMERSYS;
    timerarm->load = RATE_TIMERARM;
    timerarm->ctrl = BIT23_TIMERARM | EN_TIMERARM |
                     INTEN_TIMERARM | PRE256_TIMERARM;

    klog_ok("Timers initialized");
}

uint64_t get_time(void)
{
    uint64_t time = timersys->high;
    time <<= 32;
    time |= timersys->low;
    return time;
}

void usleep(uint32_t musec)
{
    uint32_t t = timersys->low;
    while (timersys->low - t < musec);
}
