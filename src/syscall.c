#include "syscall.h"

uint32_t syscall_table[NB_SYS];

static void fake_syscall(void)
{
    klog("Syscall invoked\n");
}

void init_syscalls(void)
{
    syscall_table[0] = (uint32_t)&fake_syscall;

    klog_ok("Syscall table initialized");
}
