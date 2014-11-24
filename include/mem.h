#ifndef MEM_H
# define MEM_H

# include <stdint.h>
# include "console.h"

/*
** For the moment, I don't find out how to write a real memory management on rpi's architecture,
** so I'll just try to do it, as a ppage allocator, and I'll improve it later...
**
*/

# define PAGE_SIZE       4096
# define PAGE_ROOT       (void *)0xFF000
# define NULL            0x00000000

void init_mem(void);
void *sbrk(uint32_t increment);
void *malloc(uint32_t size);

int brk;

#endif /* !MEM_H */
