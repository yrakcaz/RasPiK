#ifndef MMIO_H
# define MMIO_H

# include <stdint.h>

// Write value to mmio register.
void outb(uint32_t reg, uint32_t val); //Syscall
// Read value from mmio register.
uint32_t inb(uint32_t reg); //Syscall

static inline void sync_mem(void)
{
    asm ("mcr p15, 0, ip, c7, c5, 0");
    asm ("mcr p15, 0, ip, c7, c5, 6");
    asm ("mcr p15, 0, ip, c7, c10, 4");
    asm ("mcr p15, 0, ip, c7, c5, 4");
}

static inline void dmb(void)
{
    asm volatile ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0));
}

static inline void dsb(void)
{
    asm volatile ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0));
}

static inline void flush(void)
{
    asm volatile ("mcr p15, #0, %[zero], c7, c14, #0" : : [zero] "r" (0));
}

#endif /* !MMIO_H */
