#ifndef UTILS_H
# define UTILS_H

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

extern uint64_t st_read(void);
extern void st_delay(uint64_t offset, uint64_t musec);

/* Loop musec microseconds doing anything. */
static inline void wait(uint64_t musec)
{
    st_delay(st_read(), musec);
}

/* Some usefull functions... */
uint32_t strlen(char *str);
char *itoa(int val, int base);

#endif /* !UTILS_H */
