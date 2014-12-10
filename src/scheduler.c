#include "scheduler.h"

static void switch_context(void)
{
    asm volatile("mov sp, %[addr]"
                 :: [addr]"r"((uint32_t)(current_process->sp)));
    if (current_process->nbrun > 1)
    {
        asm volatile("pop {r0, lr}");
        asm volatile("add sp, sp, r0");
        asm volatile("pop {r0-r12}");
        asm volatile("rfeia sp!");
    }
    else
    {
        asm volatile("mov r0, %0" :: "r"(current_process->pc));
        asm volatile("push {r0}");
        asm volatile("cpsie i");
        asm volatile("pop {pc}");
    }
}

void schedule(void)
{
    if (!current_process || !real_nbproc)
        return;

    /* Save current process stack */
    asm volatile("mov %0, r0\n\t" : "=r"(current_process->sp));
    if (current_process->status == RUN)
        current_process->status = WAIT;

    /* Choose next process */
    do
    {
        if (current_process->next->status == TERM)
            remove_process(current_process->next->pid);
        current_process = current_process->next;
    } while (current_process->status != WAIT);
    current_process->status = RUN;
    current_process->nbrun++;

    /* Then switch context */
    switch_context();
}

void process1(void)
{
    int i = 0;
    while (++i)
        klog(itoa(i, 10), strlen(itoa(i, 10)), GREEN);
}

void process2(void)
{
    int i = 0;
    while (++i)
        klog(itoa(i, 10), strlen(itoa(i, 10)), RED);
}

void endloop(void)
{
    while(1);
}

int init_scheduler(void)
{
    if (add_process("init", (uint32_t)&endloop) < 0)
        return 0;

    return 1;
}
