#include "interrupts.h"

static volatile uint32_t *irqbase = (uint32_t *)(IRQ_BASE + BASE);

static volatile uint32_t *timerload = (uint32_t *)(IRQ_BASE + TMLOAD);
static volatile uint32_t *timerctrl = (uint32_t *)(IRQ_BASE + TMCTRL);
static volatile uint32_t *timerclear = (uint32_t *)(IRQ_BASE + TMCLR);
static uint32_t tick = 0; //Represents approximatively the number of seconds since the interrupts init.

void treat_undef(void)
{
    write_console("Bad Exception handled: UNDEF!\n", 30, RED);
    while (1) {}
}

void treat_unused(void)
{
    write_console("Bad Exception handled: UNUSED!\n", 31, RED);
    while (1) {}
}

void treat_fiq(void)
{
    write_console("Bad Exception handled: FIQ!\n", 28, RED);
    while (1) {}
}

extern void syscall_handler(uint32_t addr);

void treat_swi(void)
{
    uint32_t number = 0;
    asm volatile ("ldrb %0, [lr, #-2]" : "=r" (number));
    syscall_handler(syscall_table[number]);
}

void treat_pref_abort(void)
{
    uint32_t addr;
    asm volatile ("mov %[addr], lr"
                 : [addr]"=r"(addr));

    write_console("PREFETCH ABORT!\n", 16, BLUE);

    while (1) {};
}

void treat_data_abort(void)
{
    uint32_t addr, fault_addr;

    asm volatile("mov %[addr], lr"
                 : [addr]"=r"(addr));
    asm volatile("mrc p15, 0, %[addr], c6, c0, 0"
                 : [addr]"=r"(fault_addr));

    write_console("DATA ABORT!\n", 12, BLUE);
    while (1) {}
}

void treat_irq(void)
{
    static int i = 0;
    i++;
    if (i == 4)
    {
        i = 0;
        tick++;
    }
    *timerclear = 1; //Will be modified if another IRQ needed.
}

uint32_t gettick(void)
{
    return tick;
}

extern void vector();

void init_interrupts(void)
{
    asm volatile ("mcr p15, 0, %[addr], c12, c0, 00"
                  :: [addr]"r"(&vector));
    asm volatile ("cpsie i");

    *irqbase = BASEVAL;
    *timerload = LOADVAL;
    *timerctrl = CTRLVAL;
}
