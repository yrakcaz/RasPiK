#include "scheduler.h"

// Experimental. Without MMU support there is no address space isolation, stacks
// are tiny, and processes can freely corrupt each other. Do not rely on this for
// anything beyond basic testing.

static void switch_context(void)
{
    asm volatile("mov sp, %[addr]"
                 :: [addr]"r"((uint32_t)(current_process->sp)));
    if (current_process->run_count > 1)
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
        asm volatile("mov r0, %0" :: "r"(current_process->r0));
        asm volatile("mov r1, %0" :: "r"(current_process->r1));
        asm volatile("cpsie i");
        asm volatile("pop {pc}");
    }
}

void schedule(void)
{
    if (!current_process || !real_nbprocs)
        return;

    asm volatile("mov %0, r0\n\t" : "=r"(current_process->sp));
    if (current_process->status == RUN)
        current_process->status = WAIT;

    do
    {
        if (current_process->next->status == TERM)
        {
            if (!current_process->next->wait_time || current_process->next->wait_time == USEC_PER_SEC)
                remove_process(current_process->next->pid);
            else
                current_process->next->wait_time++;
        }
        current_process = current_process->next;
    } while (current_process->status != WAIT);
    current_process->status = RUN;
    current_process->run_count++;

    switch_context();
}

static void endloop(void)
{
    while(1);
}

int init_scheduler(void)
{
    if (add_process("init", (uint32_t)&endloop, NULL, WAIT) < 0)
    {
        klog_ko("Scheduler initialization failed");
        return 0;
    }

    klog_ok("Scheduler initialized");

    return 1;
}
