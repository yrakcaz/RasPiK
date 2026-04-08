#include "interrupts.h"

s_interrupts *irq_controller;

void handle_undef(void)
{
    klogc("Unhandled exception: UNDEF\n", RED);
    while (1);
}

void handle_unused(void)
{
    klogc("Unhandled exception: UNUSED\n", RED);
    while (1);
}

void handle_fiq(void)
{
    klogc("Unhandled exception: FIQ\n", RED);
    while (1);
}

void handle_swi(int r0, int r1, int r2, int r3)
{
    uint32_t number = 0;
    asm volatile ("ldrb %0, [lr, #-2]" : "=r" (number));
    ((funcptr)syscall_table[number])(r0, r1, r2, r3);
}

void handle_pref_abort(void)
{
    klogc("Prefetch abort\n", RED);
    while (1);
}

void handle_data_abort(void)
{
    uint32_t addr, fault_addr;

    asm volatile("mov %[addr], lr"
                 : [addr]"=r"(addr));
    asm volatile("mrc p15, 0, %[addr], c6, c0, 0"
                 : [addr]"=r"(fault_addr));

    klogc("Data abort\n", RED);
    while (1);
}

void handle_irq(void)
{
    timerarm->clr_irq = IRQ_TIMERARM;
    schedule();
}

static void __attribute__((naked, aligned(32))) vector(void)
{
    asm volatile(
                 "b handle_reset\n"
                 "b handle_undef\n"
                 "b swi_handler\n"
                 "b handle_pref_abort\n"
                 "b handle_data_abort\n"
                 "b handle_unused\n"
                 "b irq_handler\n"
                 "b handle_fiq\n"
                );
}

void init_interrupts(void)
{
    irq_controller = (s_interrupts *)BASE_INTERRUPTS;

    asm volatile ("mcr p15, 0, %[addr], c12, c0, 0"
                  :: [addr]"r"(&vector));
    asm volatile ("cpsie i");

    irq_controller->irq_en0 = IRQ_TIMERARM;

    klog_ok("Interrupt vector initialized");
}
