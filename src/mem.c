#include "../include/mem.h"

void init_mem(void)
{
    brk = 0;
    nb_page = 0;
}

void *get_next_page(void)
{
    void *ret = (void *)((char *)PAGE_ROOT + (nb_page * PAGE_SIZE));
    nb_page++;
    return ret;
}

void *sbrk(uint32_t increment)
{
    if (increment % PAGE_SIZE)
    {
        kwrite("Error while allocating memory : wrong incrment SIZE!\n", 53, RED);
        return NULL;
    }
    void *ret = (void *)((char *)PAGE_ROOT + brk);
    nb_page += (increment / PAGE_SIZE);
    brk += increment;
    return ret;
}
