#include "../include/mem.h"

void init_mem(void)
{
    brk = 0;
}

void *sbrk(uint32_t increment)
{
    void *ret = (void *)((char *)PAGE_ROOT + brk);
    brk += increment;
    return ret;
}

void *malloc(uint32_t size)
{
    return sbrk(size);
}
