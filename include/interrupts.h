#ifndef INTERRUPTS_H
# define INTERRUPTS_H

# include "console.h"

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
