#ifndef MEM_H
# define MEM_H

# include <stdint.h>
# include "klog.h"

/*
** For the moment, I don't find out how to write a real memory management on rpi's architecture,
** so I'll just try to do it with a simple sbrk before to add a real segmentation, paging or
** whatever...
*/

# define NULL    0x00000000
# define NBALLOC 1024

typedef union header
{
    struct
    {
        union header *next;
        unsigned len;
    } metadata;
    long align;
} u_header;

void *sbrk(uint32_t increment); //Syscall

void *kmalloc(uint32_t size);
void kfree(void *ptr);
void *kcalloc(uint32_t num, uint32_t len);
void *krealloc(void *ptr, uint32_t size);

#endif /* !MEM_H */
