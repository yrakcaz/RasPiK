#ifndef SYSCALL_H
# define SYSCALL_H

# include <stdint.h>
# include "timers.h"
# include "klog.h"
# include "process.h"

# define NB_SYS 23

# define SYS_SBRK 0
# define SYS_OUTB 1
# define SYS_INB  2
# define SYS_KILL 3
# define SYS_EXIT 4
# define SYS_FCLL 5
# define SYS_WAIT 6
# define SYS_FEXE 7
# define SYS_GTIM 8
# define SYS_EXEC 9
# define SYS_OPEN 10
# define SYS_CLOS 11
# define SYS_READ 12
# define SYS_WRIT 13
# define SYS_RMV  14
# define SYS_RDIR 15
# define SYS_SEEK 16
# define SYS_IOCT 17
# define SYS_STAT 18
# define SYS_MNT  19
# define SYS_UMNT 20
# define SYS_CHMD 21
# define SYS_INMD 22

void init_syscalls(void);

uint32_t syscall_table[NB_SYS];

#endif /* !SYSCALL_H */
