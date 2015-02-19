#include "libraspik.h"

extern int test(const char *str);
extern void *sbrk(uint32_t inctrement);
extern int open(const char *path, int mode);
extern int write(int fd, const void *buf, uint32_t len);
extern const char **readdir(const char *path);
