#include "fs/vfs.h"
#include "mem.h"
#include "syscalls.h"

uint32_t syscall_table[NB_SYS];

void init_syscalls(void)
{
    syscall_table[BRK] = (uint32_t)&brk;
    syscall_table[CLONE] = (uint32_t)&clone;
    syscall_table[SPAWN] = (uint32_t)&spawn;
    syscall_table[EXEC] = (uint32_t)&exec;
    syscall_table[EXIT] = (uint32_t)&exit;
    syscall_table[KILL] = (uint32_t)&kill;
    syscall_table[GETPID] = (uint32_t)&getpid;
    syscall_table[WAITPID] = (uint32_t)&waitpid;
    syscall_table[GETTIME] = (uint32_t)&gettime;
    syscall_table[OPEN] = (uint32_t)&open;
    syscall_table[CLOSE] = (uint32_t)&close;
    syscall_table[READ] = (uint32_t)&read;
    syscall_table[WRITE] = (uint32_t)&write;
    syscall_table[SEEK] = (uint32_t)&seek;
    syscall_table[STAT] = (uint32_t)&stat;
    syscall_table[REMOVE] = (uint32_t)&remove;
    syscall_table[READDIR] = (uint32_t)&readdir;
    syscall_table[IOCTL] = (uint32_t)&ioctl;
    syscall_table[INSMOD] = (uint32_t)&insmod;
    syscall_table[MOUNT] = (uint32_t)&mount;
    syscall_table[UNMOUNT] = (uint32_t)&unmount;
    syscall_table[CHMOD] = (uint32_t)&chmod;

    klog_ok("Syscall table initialized");
}
