#include "../include/interrupts.h"

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

void treat_swi(void) {}
void treat_pref_abort(void) {}
void treat_data_abort(void) {}
void treat_irq(void) {}

