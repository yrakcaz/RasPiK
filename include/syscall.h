#ifndef SYSCALL_H
# define SYSCALL_H

# include <stdint.h>
# include "timers.h"
# include "klog.h"
# include "process.h"

# define NB_SYS 24

# define SYS_KLOG 0
# define SYS_SBRK 1
# define SYS_OUTB 2
# define SYS_INB  3
# define SYS_KILL 4
# define SYS_EXIT 5
# define SYS_FCLL 6
# define SYS_WAIT 7
# define SYS_FEXE 8
# define SYS_GTIM 9
# define SYS_EXEC 10
# define SYS_OPEN 11
# define SYS_CLOS 12
# define SYS_READ 13
# define SYS_WRIT 14
# define SYS_RMV  15
# define SYS_RDIR 16
# define SYS_SEEK 17
# define SYS_IOCT 18
# define SYS_STAT 19
# define SYS_MNT  20
# define SYS_UMNT 21
# define SYS_CHMD 22
# define SYS_INMD 23

void init_syscalls(void);

uint32_t syscall_table[NB_SYS];

#endif /* !SYSCALL_H */
