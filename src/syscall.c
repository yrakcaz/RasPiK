#include "syscall.h"

void init_syscalls(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    syscall_table[SYS_KLOG] = (uint32_t)&klog;
    syscall_table[SYS_SBRK] = (uint32_t)&sbrk;
    syscall_table[SYS_OUTB] = (uint32_t)&outb;
    syscall_table[SYS_INB] = (uint32_t)&inb;
    syscall_table[SYS_KILL] = (uint32_t)&kill;
    syscall_table[SYS_EXIT] = (uint32_t)&exit;
    syscall_table[SYS_FCLL] = (uint32_t)&fork_call;
    syscall_table[SYS_WAIT] = (uint32_t)&wait;
    syscall_table[SYS_FEXE] = (uint32_t)&fork_exec;
    syscall_table[SYS_GTIM] = (uint32_t)&get_time;
    syscall_table[SYS_EXEC] = (uint32_t)&exec;
    syscall_table[SYS_OPEN] = (uint32_t)&open;
    syscall_table[SYS_CLOS] = (uint32_t)&close;
    syscall_table[SYS_READ] = (uint32_t)&read;
    syscall_table[SYS_WRIT] = (uint32_t)&write;
    syscall_table[SYS_RMV] = (uint32_t)&remove;
    syscall_table[SYS_RDIR] = (uint32_t)&readdir;
    syscall_table[SYS_SEEK] = (uint32_t)&seek;
    syscall_table[SYS_IOCT] = (uint32_t)&ioctl;
    syscall_table[SYS_STAT] = (uint32_t)&stat;
    syscall_table[SYS_MNT] = (uint32_t)&mount;
    syscall_table[SYS_UMNT] = (uint32_t)&unmount;
    syscall_table[SYS_CHMD] = (uint32_t)&chmod;
    syscall_table[SYS_INMD] = (uint32_t)&insmod;

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tSyscall table initialized!\n", 29, WHITE);
}
