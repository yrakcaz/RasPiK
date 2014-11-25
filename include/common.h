#ifndef COMMON_H
# define COMMON_H

# include <stdint.h>
# include "console.h"
# include "mem.h"

/* Shut up compiler! */
# define DO_NOTHING_WITH(X) (void)(X)

/* Masks */
# define MASK32     0x00000000
# define DATAMASK32 0xFFFFFFF0

/* Status */
# define ERROR32    0xFFFFFFFF

/* Loop ncycles processor cycles doing anything. */
static inline void wait(uint32_t ncycles)
{
    while (ncycles-- > 0)
        asm volatile ("mov r0, r0");
}

/* Some usefull functions... */
uint32_t strlen(char *str);
char *itoa(int val, int base);

#endif /* !COMMON_H */
