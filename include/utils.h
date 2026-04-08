#ifndef UTILS_H
# define UTILS_H

# include <stdint.h>

# include "klog.h"
# include "mem.h"

# define DO_NOTHING_WITH(X) (void)(X)

# define MASK32     0x00000000
# define DATAMASK32 0xFFFFFFF0

# define ERROR32    0xFFFFFFFF

uint32_t strlen(const char *str);
char *itoa(int val, int base);
char *strcat(const char *str1, const char *str2);
int strcmp(const char *str1, const char *str2);
void uint32_to_hex(uint32_t val, char *buf);

#endif /* !UTILS_H */
