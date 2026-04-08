#ifndef SYSCALL_H
# define SYSCALL_H

# include <stdint.h>

# include "klog.h"
# include "timers.h"

# define NB_SYS 1

void init_syscalls(void);

extern uint32_t syscall_table[NB_SYS];

#endif /* !SYSCALL_H */
