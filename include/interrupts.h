#ifndef INTERRUPTS_H
# define INTERRUPTS_H

# include "console.h"
# include "mem.h"
# include "syscall.h"

/* Usefull defines for interrupts.. */
# define IRQ_BASE 0x2000B000

# define BASE     0x218

# define TMLOAD   0x400
# define TMCTRL   0x408
# define TMCLR    0x40C

# define BASEVAL  0x00000001
# define LOADVAL  0x00000400

# define BITS23   (1 << 1)
# define PRE256   (2 << 2)
# define INTEN    (1 << 5)
# define ENABLE   (1 << 7)

# define CTRLVAL  (BITS23 | PRE256 | INTEN | ENABLE)

/* Initialization */
void init_interrupts(void);

/* Bad exceptions */
void treat_undef(void);
void treat_unused(void);
void treat_fiq(void);

/* Other interrupts */
void treat_swi(void);
void treat_pref_abort(void);
void treat_data_abort(void);
void treat_irq(void);

uint32_t gettick(void); // Returns number of seconds since interrupts init...

#endif /* !INTERRUPTS_H */
