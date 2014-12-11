#ifndef SYSCALL_H
# define SYSCALL_H

# include <stdint.h>
# include "timers.h"
# include "klog.h"

# define NB_SYS 1

void init_syscalls(void);

uint32_t syscall_table[NB_SYS];

#endif /* !SYSCALL_H */
