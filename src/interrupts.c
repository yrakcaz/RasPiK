#include "../include/interrupts.h"

static volatile uint32_t *irq1 = (uint32_t *)(IRQ_BASE + IRQ1);
static volatile uint32_t *irq2 = (uint32_t *)(IRQ_BASE + IRQ2);
static volatile uint32_t *irqbase = (uint32_t *)(IRQ_BASE + BASE);

static volatile uint32_t *timerload = (uint32_t *)(IRQ_BASE + TMLOAD);
static volatile uint32_t *timervalue = (uint32_t *)(IRQ_BASE + TMVAL);
static volatile uint32_t *timerctrl = (uint32_t *)(IRQ_BASE + TMCTRL);
static volatile uint32_t *timerclear = (uint32_t *)(IRQ_BASE + TMCLR);

void treat_reset(void)
{
    kwrite("Bad Exception handled: RESET!\n", 30, RED);
    while (1) {}
}

void treat_undef(void)
{
    kwrite("Bad Exception handled: UNDEF!\n", 30, RED);
    while (1) {}
}

void treat_unused(void)
{
    kwrite("Bad Exception handled: UNUSED!\n", 31, RED);
    while (1) {}
}

void treat_fiq(void)
{
    kwrite("Bad Exception handled: FIQ!\n", 28, RED);
    while (1) {}
}

void treat_swi(void) 
{
    uint32_t addr;
    uint32_t number;

    asm volatile ("mov %[addr], lr"
                  : [addr]"=r"(addr));
    addr -= 4;
    number = *((uint32_t *)addr) & 0x00FFFFFF;
    kwrite("SWI\n", 4, BLUE);
}

void treat_pref_abort(void) 
{
    uint32_t addr;
    asm volatile ("mov %[addr], lr"
                 : [addr]"=r"(addr));

    kwrite("PREF\n", 5, BLUE);

    //endloop?
}

void treat_data_abort(void) 
{
    uint32_t addr, fault_addr;

    asm volatile("mov %[addr], lr"
                 : [addr]"=r"(addr));
    asm volatile("mrc p15, 0, %[addr], c6, c0, 0"
                 : [addr]"=r"(fault_addr));

    kwrite("DATA\n", 5, BLUE);
}

void treat_irq(void) 
{
    *timerclear = 0;
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
    DO_NOTHING_WITH(irq1);
    DO_NOTHING_WITH(irq2);
    DO_NOTHING_WITH(timervalue);
}
