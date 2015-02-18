#include <mmio.h>

void outb(uint32_t reg, uint32_t val)
{
    uint32_t *addr = (uint32_t *)reg;
    asm volatile(
                     "str %[val], [%[reg]]"
                     :: [reg]"r"(addr), [val]"r"(val)
                    );
}

uint32_t inb(uint32_t reg)
{
    uint32_t *addr = (uint32_t *)reg;
    uint32_t ret;
    asm volatile(
                     "ldr %[ret], [%[reg]]"
                     : [ret]"=r"(ret)
                     : [reg]"r"(addr)
                    );
    return ret;
}
