#include "mem.h"

extern char *get_sp();

void *sbrk(uint32_t increment)
{
    extern char _end;
    static char *heap_end;
    char *previous_heap_end;

    if (!heap_end)
        heap_end = &_end;

    previous_heap_end = heap_end;

    if (heap_end + increment > get_sp())
    {
        write_console ("Heap is full: abort!\n", 21, RED);
        while (1) {}
    }

    heap_end += increment;
    return previous_heap_end;
}

void *kmalloc(uint32_t size)
{
    return sbrk(size);
}
