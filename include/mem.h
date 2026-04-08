#ifndef MEM_H
# define MEM_H

# include <stdint.h>

# include "klog.h"

// Memory management is currently implemented as a simple brk-based allocator.
// A proper implementation would add paging and MMU support for the ARM1176JZF-S.

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

int brk(void *addr);

void kfree(void *ptr);
void *kmalloc(uint32_t size);
void *kcalloc(uint32_t num, uint32_t len);
void *krealloc(void *ptr, uint32_t size);

#endif /* !MEM_H */
