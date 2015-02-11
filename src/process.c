#include "process.h"

static uint32_t stacks[NBMAX_PROC];

extern uint32_t get_sp(void);

int add_process(const char *name, uint32_t pc, int status/* stdio later...*/)
{
    s_proc *process = kmalloc(sizeof (s_proc));
    if (!process)
        return -1;

    DISABLE_INTERRUPTS();

    process->name = name;
    process->pid = nbproc;
    process->ppid = !current_process ? -1 : current_process->pid;
    process->status = status;
    process->nbrun = 0;
    process->pc = pc;

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
    //TODO : STDIOS here!

    switch (real_nbproc)
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
    nbproc++;
    real_nbproc++;

    //TODO : Create corresponding file?

    if (real_nbproc == 1)
        current_process->nbrun++;

    ENABLE_INTERRUPTS();

    return process->pid;
}

static int copy_stack(int dstpid, int srcpid)
{
    if (dstpid < 0 || srcpid < 0 || srcpid == dstpid)
        return -1;

    int src = -1;
    int dst = -1;

    s_proc *proc = current_process;
    int base = proc->pid;
    do
    {
        if (proc->pid == dstpid)
            dst = proc->stack_idx;
        if (proc->pid == srcpid)
            src = proc->stack_idx;

        proc = proc->next;
    } while (proc->pid != base);

    if (src < 0 || dst < 0)
        return -1;

    char *dststack = ((char *)stacks[dst]);
    char *srcstack = ((char *)stacks[src]);

    for (int i = 0; i < STACK_SIZE; i++)
        dststack[i] = srcstack[i];

    return STACK_SIZE;
}

static void child(void)
{
    asm volatile ("mov lr, %0\n\t" :: "r"(0));
    asm volatile ("pop {pc}\n\t");
}

int fork(void)
{

    //TODO : Try to re-write fork without add_process, like in linux 0.10
    //       --> Find empty process then copy process etc.
    //       --> ASM Wrapper

    char *name = strcat("undefined", itoa(nbproc, 10));

    int newpid = add_process(name, (uint32_t)&child, SLEEP);
    if (newpid < 0)
        return -1;

    if (copy_stack(newpid, current_process->pid) < 0)
    {
        remove_process(newpid);
        return -1;
    }

    kill(newpid, WAIT);

    return newpid;
}

int remove_process(int pid)
{
    s_proc *process = current_process;
    if (!process)
        return -1;

    DISABLE_INTERRUPTS();

    if (pid == 1)
    {
        klog("\n\n*** SYSTEM HALTING ***\n", 25, RED);
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
            real_nbproc--;
            if (real_nbproc == 1)
                current_process->status = WAIT;
            ENABLE_INTERRUPTS();
            return process->pid;
        }
        process = process->next;
    } while (process->pid != base);

    //TODO : Kill children processes?

    ENABLE_INTERRUPTS();
    return -1;
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
    DO_NOTHING_WITH(status); // TODO : <-- Do something with it..
    kill(current_process->pid, TERM);
}

void init_process(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    for (int i = 0; i < NBMAX_PROC; i++)
        stacks[i] = 0;
    current_process = NULL;
    nbproc = 1;
    real_nbproc = 0;

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tProcesses initialized!\n", 25, WHITE);
}
