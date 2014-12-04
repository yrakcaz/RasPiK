#include "scheduler.h"

static uint32_t stack_base;

extern void end_irq(void);

void schedule(uint32_t pc, uint32_t sp)
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
                  :: [addr]"r"((uint32_t)(current_process->context.sp)));

    if (current_process->nbrun > 1)
    {
        clear_timer();
        end_irq();
    }
    else
    {
        uint32_t addr = current_process->context.pc;
        asm volatile("mov r0, %[addr]"
                     :: [addr]"r"(addr));
        asm volatile ("push {r0}");
        clear_timer();
        asm volatile ("cpsie i");
        asm volatile ("pop {pc}");
    }
}

extern uint32_t get_sp(void);

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
    stack_base = get_sp();
    s_context contextmain;
    s_context context1;
    s_context context2;
    contextmain.sp = stack_base;
    contextmain.pc = (uint32_t)&endloop;
    context1.sp = stack_base + STACK_SIZE;
    context1.pc = (uint32_t)&process1;
    context2.sp = stack_base + 2 * STACK_SIZE;
    context2.pc = (uint32_t)&process2;
    if (add_process("main", contextmain) < 0)
        return 0;
    if (add_process("process1", context1) < 0)
        return 0;
    if (add_process("process2", context2) < 0)
        return 0;
    current_process->nbrun++;
    current_process->status = ZOMBIE;
    return 1;
}
