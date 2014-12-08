#include "scheduler.h"

void schedule(void)
{
    if (!current_process || (nbproc < 2))
        return;

    /* Save current process stack */
    asm volatile("mov %0, r0\n\t" : "=r"(current_process->sp));
    if (current_process->status == RUN)
        current_process->status = WAIT;

    /* Choose next process */
    do
    {
        current_process = current_process->next;
    } while (current_process->status != WAIT);
    current_process->status = RUN;
    current_process->nbrun++;

    /* Then switch context */
    asm volatile("mov sp, %[addr]" 
                 :: [addr]"r"((uint32_t)(current_process->sp)));
    if (current_process->nbrun <= 1)
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

void process1(void)
{
    while (1)
        klog("Bonjour\n", 8, GREEN);
}

void process2(void)
{
    while (1)
        klog("Aurevoir\n", 9, YELLOW);
}

void endloop(void)
{
    while(1);
}

int init_scheduler(void)
{
    if (add_process("init", (uint32_t)&endloop) < 0)
        return 0;
    if (add_process("process1", (uint32_t)&process1) < 0)
        return 0;
    if (add_process("process2", (uint32_t)&process2) < 0)
        return 0;
    current_process->nbrun++;
    current_process->status = ZOMBIE;

    return 1;
}
