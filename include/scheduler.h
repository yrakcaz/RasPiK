#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "process.h"
# include "interrupts.h"

void schedule(void);
int init_scheduler(void);

//DEBUG:
void process1(void);
void process2(void);

#endif /* !SCHEDULER_H */
