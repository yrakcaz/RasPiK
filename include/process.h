#ifndef PROCESS_H
# define PROCESS_H

# include <stdint.h>

# include "elf.h"
# include "fs/vfs.h"
# include "mem.h"
# include "timers.h"

# define NBMAX_PROC 4096
# define NBMAX_FD   4096
# define STACK_SIZE 2048

# define ENABLE_INTERRUPTS() \
    asm volatile("cpsie i");
# define DISABLE_INTERRUPTS() \
    asm volatile("cpsid i");

typedef enum
{
    RUN,
    WAIT,
    ZOMBIE,
    TERM,
    SLEEP,
} e_pstatus;

typedef struct proc
{
    const char *name;
    int pid;
    int ppid;
    e_pstatus status;
    int run_count;
    int stack_idx;
    uint32_t pc;
    uint32_t sp;
    uint32_t r0;
    uint32_t r1;
    int retval;
    int wait_time;
    s_fd fd_table[NBMAX_FD];
    struct proc *next;
    struct proc *prev;
} s_proc;

extern s_proc *current_process;
extern int nbprocs;
extern int real_nbprocs;

int add_process(const char *name, uint32_t pc, char **args, int status);
int remove_process(int pid);
int clone(uint32_t addr, char **args);
int spawn(const char *path, char **args);
void exit(int status);
int kill(int pid, int status);
int getpid(void);
int waitpid(int pid, int *retval);
void init_process(void);

#endif /* !PROCESS_H */
