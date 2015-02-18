#include "libraspik.h"

void *sbrk(uint32_t increment)
{
    void *ret;
    asm volatile ("mov r0, %0" :: "r"(increment));
    asm volatile ("swi #0");
    asm volatile ("mov %0, r0" : "=r"(ret));
    return ret;
}

int open(const char *path, int mode)
{
    int ret;
    asm volatile ("mov r0, %0" :: "r"(path));
    asm volatile ("mov r1, %0" :: "r"(mode));
    asm volatile ("swi #10");
    asm volatile ("mov %0, r0" : "=r"(ret));
    return ret;
}

int write(int fd, const void *buf, uint32_t len)
{
    int ret;
    asm volatile ("mov r0, %0" :: "r"(fd));
    asm volatile ("mov r1, %0" :: "r"(buf));
    asm volatile ("mov r2, %0" :: "r"(len));
    asm volatile ("swi #13");
    asm volatile ("mov %0, r0" : "=r"(ret));
    return ret;
}

const char **readdir(const char *path)
{
    const char **ret;
    asm volatile ("mov r0, %0" :: "r"(path));
    asm volatile ("swi #15");
    asm volatile ("mov %0, r0" : "=r"(ret));
    return ret;
}
