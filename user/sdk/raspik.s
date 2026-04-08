.global _start
_start:
    bl main
    bl exit
0:  b 0b

.macro SYSCALL name, num
.global \name
\name:
    swi #\num
    bx lr
.endm

SYSCALL brk, 0
SYSCALL clone, 1
SYSCALL spawn, 2
SYSCALL exec, 3
SYSCALL exit, 4
SYSCALL kill, 5
SYSCALL getpid, 6
SYSCALL waitpid, 7
SYSCALL gettime, 8
SYSCALL open, 9
SYSCALL close, 10
SYSCALL read, 11
SYSCALL write, 12
SYSCALL seek, 13
SYSCALL stat, 14
SYSCALL remove, 15
SYSCALL readdir, 16
SYSCALL ioctl, 17
SYSCALL insmod, 18
SYSCALL mount, 19
SYSCALL unmount, 20
SYSCALL chmod, 21
