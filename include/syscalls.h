#ifndef SYSCALLS_H
# define SYSCALLS_H

# include <stdint.h>

# include "process.h"
# include "timers.h"

typedef enum
{
    // memory
    BRK = 0,
    // process
    CLONE,
    SPAWN,
    EXEC,
    EXIT,
    KILL,
    GETPID,
    WAITPID,
    // time
    GETTIME,
    // file
    OPEN,
    CLOSE,
    READ,
    WRITE,
    SEEK,
    STAT,
    REMOVE,
    READDIR,
    // device
    IOCTL,
    INSMOD,
    // vfs
    MOUNT,
    UNMOUNT,
    CHMOD,
    NB_SYS
} e_syscall;

extern uint32_t syscall_table[NB_SYS];

void init_syscalls(void);

#endif /* !SYSCALLS_H */
