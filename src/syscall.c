#include "syscall.h"

static void fake_syscall(void)
{
    write_console("Syscall test!", 13, GREEN);
}

void init_syscalls(void)
{
    syscall_table[0] = (uint32_t)&fake_syscall;
}
