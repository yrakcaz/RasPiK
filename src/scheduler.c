#include "scheduler.h"

static uint64_t stack_base;

extern void end_irq(void);

void schedule(uint64_t pc, uint64_t sp)
{
    if (!current_process)
    {
        clear_timer();
        end_irq();
        return;
    }
    current_process->context.pc = pc;
    current_process->context.sp = sp;
    if (current_process->status == RUN)
        current_process->status = WAIT;

    do
    {
        current_process = current_process->next;
    } while (current_process->status != WAIT);
    current_process->status = RUN;
    current_process->nbrun++;

    asm volatile ("mov sp, %[addr]"
                  :: [addr]"r"((uint64_t)(current_process->context.sp)));

    if (current_process->nbrun > 1)
    {
        clear_timer();
        end_irq();
    }
    else
    {
        uint64_t addr = current_process->context.pc;
        asm volatile("mov r0, %[addr]"
                     :: [addr]"r"(addr));
        asm volatile ("push {r0}");
        clear_timer();
        asm volatile ("cpsie i");
        asm volatile ("pop {pc}");
    }
}

extern uint64_t get_sp(void);

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

int init_scheduler(void)
{
    stack_base = get_sp();
    s_context context1;
    s_context context2;
    context1.sp = stack_base;
    context1.pc = (uint32_t)&process1;
    context2.sp = stack_base + (STACK_SIZE * nbproc);
    context2.pc = (uint32_t)&process2;
    if (add_process("process1", context1) < 0)
        return 0;
    if (add_process("process2", context2) < 0)
        return 0;
    return 1;
}
