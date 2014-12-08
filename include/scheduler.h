#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "process.h"
# include "interrupts.h"

void schedule(void);
int init_scheduler(void);

#endif /* !SCHEDULER_H */
