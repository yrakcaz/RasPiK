#ifndef INTERRUPTS_H
# define INTERRUPTS_H

# include "console.h"
# include "mem.h"

# define IRQ_BASE 0x2000B000

# define IRQ1     0x210
# define IRQ2     0x214
# define BASE     0x218

# define TMLOAD   0x400
# define TMVAL    0x404
# define TMCTRL   0x408
# define TMCLR    0x40C

# define BASEVAL  0x00000001
# define LOADVAL  0x00000400
# define CTRLVAL  0x000000AA

/* Initialization */
void init_interrupts(void);

/* Bad exceptions */
void treat_reset(void);
void treat_undef(void);
void treat_unused(void);
void treat_fiq(void);

/* Other interrupts */
void treat_swi(void);
void treat_pref_abort(void);
void treat_data_abort(void);
void treat_irq(void);

#endif /* !INTERRUPTS_H */
