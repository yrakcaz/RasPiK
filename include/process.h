#ifndef PROCESS_H
# define PROCESS_H

# include "io.h"

typedef struct context
{
    uint64_t sp;
    uint64_t pc;
} s_context;

typedef struct proc
{
    const char *name;
    int pid;
    int status;
    s_context context;
    s_fd fd_table[NBMAX_FD];
    struct proc *next;
    struct proc *prev;
} s_proc;

s_proc *current_process;

#endif /* !PROCESS_H */
