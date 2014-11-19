#ifndef MMIO_H
# define MMIO_H

# include <stdint.h>

// Write value to mmio register.
static inline void write_mmio(uint32_t reg, uint32_t val)
{
    uint32_t *addr = (uint32_t *)reg;
    __asm__ volatile(
                     "str %[val], [%[reg]]"
                     :: [reg]"r"(addr), [val]"r"(val)
                    );
}

// Read value from mmio register.
static inline uint32_t read_mmio(uint32_t reg)
{
    uint32_t *addr = (uint32_t *)reg;
    uint32_t ret;
    __asm__ volatile(
                     "ldr %[ret], [%[reg]]"
                     : [ret]"=r"(ret)
                     : [reg]"r"(addr)
                    );
    return ret;
}

#endif /* !MMIO_H */
