#ifndef PROCESS_H
# define PROCESS_H

# include "io.h"

# define RUN    0
# define WAIT   1
# define ZOMBIE 2
# define TERM   3
# define ABORT  4

# define NBMAX_PROC 4096
# define STACK_SIZE 2048

typedef struct proc
{
    const char *name;
    int pid;
    int ppid;
    int status;
    int nbrun;
    int stack_idx;
    uint32_t pc;
    uint32_t sp;
    s_fd fd_table[NBMAX_FD];
    struct proc *next;
    struct proc *prev;
} s_proc;

s_proc *current_process;
int nbproc;

int add_process(const char *name, uint32_t pc/* stdio later... */);
int remove_process(int pid);
int kill(int pid, int status);
void init_process(void);

#endif /* !PROCESS_H */
