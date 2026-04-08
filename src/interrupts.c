#include "interrupts.h"

s_interrupts *irq_controller;

void handle_undef(void)
{
    uint32_t addr;
    char buf[11];
    asm volatile("mov %[addr], lr" : [addr]"=r"(addr));
    uint32_to_hex(addr - 4, buf);
    klog_color("Undefined instruction: PC=", RED);
    klog_color(buf, RED);
    klog_color("\n", RED);
    while (1);
}

void handle_unused(void)
{
    klog_color("Unhandled exception: UNUSED\n", RED);
    while (1);
}

void handle_fiq(void)
{
    klog_color("Unhandled exception: FIQ\n", RED);
    while (1);
}

void handle_swi(unsigned number, unsigned *reg)
{
    reg[0] = ((funcptr)syscall_table[number])(reg[0], reg[1], reg[2], reg[3]);
}

void handle_pref_abort(void)
{
    uint32_t addr;
    char buf[11];
    asm volatile("mov %[addr], lr" : [addr]"=r"(addr));
    uint32_to_hex(addr - 4, buf);
    klog_color("Prefetch abort: PC=", RED);
    klog_color(buf, RED);
    klog_color("\n", RED);
    while (1);
}

void handle_data_abort(void)
{
    uint32_t addr, fault_addr;
    char buf[11];

    asm volatile("mov %[addr], lr"
                 : [addr]"=r"(addr));
    asm volatile("mrc p15, 0, %[addr], c6, c0, 0"
                 : [addr]"=r"(fault_addr));

    uint32_to_hex(addr - 8, buf);
    klog_color("Data abort: PC=", RED);
    klog_color(buf, RED);
    uint32_to_hex(fault_addr, buf);
    klog_color(" fault_addr=", RED);
    klog_color(buf, RED);
    klog_color("\n", RED);
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
