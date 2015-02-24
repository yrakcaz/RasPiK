#ifndef INTERRUPTS_H
# define INTERRUPTS_H

# include "klog.h"
# include "mem.h"
# include "syscall.h"
# include "timers.h"
# include "scheduler.h"

# define BASE_INTERRUPTS 0x2000B200

# define IRQ_TIMERARM    (1 << 0)
# define IRQ_MAILBOX     (1 << 1)
# define IRQ_BELL0       (1 << 2)
# define IRQ_BELL1       (1 << 3)
# define IRQ_GPU0        (1 << 4)
# define IRQ_GPU1        (1 << 5)
# define IRQ_ERROR1      (1 << 6)
# define IRQ_ERROR0      (1 << 7)

typedef struct interrupts
{
    volatile uint32_t irq_pend0;
    volatile uint32_t irq_pend1;
    volatile uint32_t irq_pend2;
    volatile uint32_t fiq_ctrl;
    volatile uint32_t irq_en1;
    volatile uint32_t irq_en2;
    volatile uint32_t irq_en0;
    volatile uint32_t irq_dis1;
    volatile uint32_t irq_dis2;
    volatile uint32_t irq_dis0;
} s_interrupts;

s_interrupts *interrupts;

typedef void (*funcptr)(int, int, int, int);

/* Initialization */
void init_interrupts(void);

/* Bad exceptions */
void treat_undef(void);
void treat_unused(void);
void treat_fiq(void);

/* Other interrupts */
void treat_swi(unsigned number, unsigned *reg);
void treat_pref_abort(void);
void treat_data_abort(void);
void treat_irq(void);

#endif /* !INTERRUPTS_H */
