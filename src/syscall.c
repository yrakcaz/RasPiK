#include "syscall.h"

static void fake_syscall(void)
{
    klog("Syscall test successful!", 24, GREEN);
}

void init_syscalls(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    syscall_table[0] = (uint32_t)&fake_syscall;

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tSyscall table initialized!\n", 29, WHITE);
}
