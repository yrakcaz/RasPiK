#include "scheduler.h"

static uint32_t stack_base;

void schedule(uint32_t pc, uint32_t sp)
{
    if (!current_process)
        return;

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

    //then switch to new context
    asm volatile ("mov sp, %[addr]"
                  :: [addr]"r"((uint32_t)(current_process->context.sp)));
    asm volatile ("mov r0, %[addr]"
                  :: [addr]"r"((uint32_t)(current_process->context.pc)));
    asm volatile ("push {r0}");
    asm volatile ("cpsie i");
    asm volatile("pop {pc}");
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

extern uint32_t get_sp(void);

int init_scheduler(void)
{
    stack_base = get_sp();

/*
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
*/

    return 1;
}
