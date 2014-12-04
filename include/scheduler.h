#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "process.h"
# include "interrupts.h"

# define STACK_SIZE 1024

void schedule(uint32_t pc, uint32_t sp);
int init_scheduler(void);

#endif /* !SCHEDULER_H */
