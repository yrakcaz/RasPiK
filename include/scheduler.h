#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "process.h"
# include "interrupts.h"

# define STACK_SIZE 1024

void schedule(uint64_t pc, uint64_t sp);
int init_scheduler(void);

#endif /* !SCHEDULER_H */
