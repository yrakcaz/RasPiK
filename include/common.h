#ifndef COMMON_H
# define COMMON_H

# include <stdint.h>

/* Masks */
# define MASK32     0x00000000
# define DATAMASK32 0xFFFFFFF0

/* Status */
# define ERROR32    0xFFFFFFFF

/* Loop ncycles processor cycles doing anything. */
static inline void wait(uint32_t ncycles)
{
   asm volatile (
                      "__wait_%=: subs %[ncycles], %[ncycles], #1; bne __wait_%=\n"
                      :: [ncycles]"r"(ncycles)
                      :  "cc"
                     );
}

#endif /* !COMMON_H */
