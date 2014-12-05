#include "process.h"

int add_process(const char *name, s_context context /* stdio later...*/)
{
    s_proc *process = kmalloc(sizeof (s_proc));
    if (!process)
        return -1;

    process->name = name;
    process->pid = nbproc;
    process->ppid = !current_process ? -1 : current_process->pid;
    process->status = WAIT;
    process->nbrun = 0;
    process->context = context;

    for (int i = 0; i < NBMAX_FD; i++)
        process->fd_table[i].inode = NULL;
    //STDIOS here!

    switch (nbproc)
    {
        case 0:
            current_process = process;
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

    //Create corresponding file?

    return process->pid;
}

int remove_process(int pid)
{
    s_proc *process = current_process;
    if (!process)
        return -1;

    int base = process->pid;

    do
    {
        if (process->pid == pid)
        {
            process->prev->next = process->next;
            process->next->prev = process->prev;
            kfree(process);
            return process->pid;
        }
        process = process->next;
    } while (process->pid != base);

    //Kill children processes?

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

void init_process(void)
{
    current_process = NULL;
    nbproc = 0;
}
