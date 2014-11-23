#include "../include/interrupts.h"

static uint32_t *irq1 = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + IRQ1);
static uint32_t *irq2 = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + IRQ2);
static uint32_t *irqbase = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + BASE);

static uint32_t *timerload = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + TMLOAD);
static uint32_t *timervalue = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + TMVAL);
static uint32_t *timerctrl = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + TMCTRL);
static uint32_t *timerclear = (uint32_t *)VIRTUAL_ADDR(IRQ_BASE + TMCLR);

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
}

void treat_pref_abort(void) 
{
}

void treat_data_abort(void) 
{
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
}

uint32_t get_timeval(void)
{
    (void)irq1;
    (void)irq2;
    return *timervalue;
}
