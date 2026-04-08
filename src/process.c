#include "process.h"

s_proc *current_process;
int nbprocs;
int real_nbprocs;

static uint32_t stacks[NBMAX_PROC];

extern uint32_t get_sp(void);

int add_process(const char *name, uint32_t pc, char **args, int status)
{
    s_proc *process = kmalloc(sizeof (s_proc));
    if (!process)
        return -1;

    DISABLE_INTERRUPTS();

    process->name = name;
    process->pid = nbprocs;
    process->ppid = !current_process ? -1 : current_process->pid;
    process->status = status;
    process->run_count = 0;
    process->pc = pc;
    current_process->retval = 0;
    current_process->wait_time = 0;

    int argc;
    for (argc = 0; args[argc]; argc++);
    process->r0 = (uint32_t)argc;
    process->r1 = (uint32_t)args;

    if (!stacks[0])
    {
        stacks[0] = get_sp();
        process->sp = stacks[0];
        process->stack_idx = 0;
    }
    else
    {
        int i;
        for (i = 0; i < NBMAX_PROC; i++)
        {
            if (!stacks[i])
            {
                stacks[i] = stacks[0] + (i * STACK_SIZE);
                process->sp = stacks[i];
                process->stack_idx = i;
                break;
            }
        }
        if (i >= NBMAX_PROC)
        {
            ENABLE_INTERRUPTS();
            kfree(process);
            return -1;
        }
    }

    for (int i = 0; i < NBMAX_FD; i++)
        process->fd_table[i].addr = NULL;

    switch (real_nbprocs)
    {
      case 0:
       current_process = process;
       current_process->next = process;
       current_process->prev = process;
       break;
      case 1:
       current_process->next = process;
       process->next = current_process;
       current_process->prev = process;
       process->prev = current_process;
       break;
      default:
       current_process->next->prev = process;
       process->next = current_process->next;
       current_process->next = process;
       process->prev = current_process;
       break;
    }
    nbprocs++;
    real_nbprocs++;

    if (real_nbprocs == 1)
        current_process->run_count++;

    ENABLE_INTERRUPTS();

    return process->pid;
}

int remove_process(int pid)
{
    s_proc *process = current_process;
    if (!process)
        return -1;

    DISABLE_INTERRUPTS();

    if (pid == 1)
    {
        klogc("System halting\n", RED);
        while (1)
            asm volatile("wfe");
    }

    int base = process->pid;

    do
    {
        if (process->pid == pid)
        {
            stacks[process->stack_idx] = 0;
            process->prev->next = process->next;
            process->next->prev = process->prev;
            kfree(process);
            real_nbprocs--;
            if (real_nbprocs == 1)
                current_process->status = WAIT;
            ENABLE_INTERRUPTS();
            return process->pid;
        }
        process = process->next;
    } while (process->pid != base);

    ENABLE_INTERRUPTS();
    return -1;
}

int fork_call(uint32_t addr, char **args)
{
    return add_process(current_process->name, addr, args, WAIT);
}

int fork_exec(const char *path, char **args)
{
    uint32_t entry = load_elf(path);
    return add_process(path, entry, args, WAIT);
}

static s_proc *get_process(int pid)
{
    s_proc *proc = current_process;
    int base = proc->pid;
    do
    {
        if (proc->pid == pid)
            return proc;
        proc = proc->next;
    } while (proc->pid != base);
    return NULL;
}

int waitpid(int pid, int *retval)
{
    s_proc *proc = get_process(pid);
    if (!pid)
        return -1;

    proc->wait_time = 1;
    while (proc->status != TERM);
    *retval = proc->retval;

    return 0;
}

int kill(int pid, int status)
{
    s_proc *process = current_process;
    if (!process)
        return -1;

    int base = process->pid;

    do
    {
        if (process->pid == pid)
        {
            process->status = status;
            return process->status;
        }
        process = process->next;
    } while (process->pid != base);

    return -1;
}

void exit(int status)
{
    current_process->retval = status;
    kill(current_process->pid, TERM);
}

void init_process(void)
{
    for (int i = 0; i < NBMAX_PROC; i++)
        stacks[i] = 0;
    current_process = NULL;
    nbprocs = 1;
    real_nbprocs = 0;

    klog_ok("Processes initialized");
}
