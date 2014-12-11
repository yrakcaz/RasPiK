#include "timers.h"

void init_timers(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    timerarm = (s_timerarm *)BASE_TIMERARM;
    timersys = (s_timersys *)BASE_TIMERSYS;
    timerarm->load = RATE_TIMERARM;
    timerarm->ctrl = BIT23_TIMERARM | EN_TIMERARM |
                     INTEN_TIMERARM | PRE256_TIMERARM;

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tTimer ARM and Timer System initialized!\n", 42, WHITE);
}

uint64_t get_time(void)
{
    uint64_t time = timersys->high;
    time <<= 32;
    time |= timersys->low;
    return time;
}

void wait(uint32_t musec)
{
    uint32_t t = timersys->low;
    while (timersys->low - t < musec) {}
}
