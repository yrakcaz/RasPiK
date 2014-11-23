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

# define VIRTUAL_ADDR(X) (X + 0x80000000)

void init_mem(void);
void *get_next_page(void);
void *sbrk(uint32_t increment);

int brk;
int nb_page;

#endif /* !MEM_H */
