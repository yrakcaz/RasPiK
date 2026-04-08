#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "interrupts.h"
# include "process.h"

void schedule(void);
int init_scheduler(void);

#endif /* !SCHEDULER_H */
