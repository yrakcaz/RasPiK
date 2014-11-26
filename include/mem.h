#ifndef MEM_H
# define MEM_H

# include <stdint.h>
# include "console.h"

/*
** For the moment, I don't find out how to write a real memory management on rpi's architecture,
** so I'll just try to do it, as a ppage allocator, and I'll improve it later...
**
*/

# define NULL 0x00000000

void *sbrk(uint32_t increment);
void *kmalloc(uint32_t size);

#endif /* !MEM_H */
