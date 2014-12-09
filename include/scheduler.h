#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "process.h"
# include "interrupts.h"

# define ENABLE_INTERRUPTS() \
    asm volatile("cpsie i");

# define DISABLE_INTERRUPTS() \
    asm volatile("cpsid i");

void schedule(void);
int init_scheduler(void);

//DEBUG:
void process1(void);

#endif /* !SCHEDULER_H */
