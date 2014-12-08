#ifndef TIMERS_H
# define TIMERS_H

# include <stdint.h>

# define BASE_TIMERARM   0x2000B400
# define BIT23_TIMERARM  (1 << 1)
# define PRE1_TIMERARM   (0 << 2)
# define PRE16_TIMERARM  (1 << 2)
# define PRE256_TIMERARM (2 << 2)
# define INTEN_TIMERARM  (1 << 5)
# define INTDIS_TIMERARM (0 << 5)
# define EN_TIMERARM     (1 << 7)
# define DIS_TIMERARM    (0 << 7)

# define RATE_TIMERARM   0x5

typedef struct timerarm
{
    volatile uint32_t load;
    volatile uint32_t val;
    volatile uint32_t ctrl;
    volatile uint32_t clr_irq;
    volatile uint32_t raw_irq;
    volatile uint32_t msk_irq;
    volatile uint32_t reload;
    volatile uint32_t prediv;
    volatile uint32_t counter;
} s_timerarm;

s_timerarm *timerarm;

# define BASE_TIMERSYS   0x20003000

typedef struct timersys
{
    volatile uint32_t status;
    volatile uint32_t low;
    volatile uint32_t high;
    volatile uint32_t cmp0;
    volatile uint32_t cmp1;
    volatile uint32_t cmp2;
    volatile uint32_t cmp3;
} s_timersys;

s_timersys *timersys;

void init_timers(void);
uint64_t get_time(void);
void wait(uint32_t musec);

#endif /* !TIMERS_H */
