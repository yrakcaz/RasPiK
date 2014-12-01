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

/* Some usefull functions... */
uint32_t strlen(const char *str);
char *itoa(int val, int base);
void wait(uint64_t musec);
char *strcat(const char *str1, const char *str2);
int strcmp(const char *str1, const char *str2);

#endif /* !UTILS_H */
