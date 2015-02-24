.global klog
.global sbrk
.global outb
.global inb
.global kill
.global exit
.global fork_call
.global waitpid
.global fork_exec
.global get_time
.global exec
.global open
.global close
.global read
.global write
.global remove
.global readdir
.global seek
.global ioctl
.global stat
.global mount
.global unmount
.global chmod
.global insmod

klog:
    push {r0-r2}
    swi #0
    bx lr

sbrk:
    push {r0}
    swi #1
    bx lr

outb:
    push {r0-r1}
    swi #2
    bx lr

inb:
    push {r0}
    swi #3
    bx lr

kill:
    push {r0-r1}
    swi #4
    bx lr

exit:
    push {r0}
    swi #5
    bx lr

fork_call:
    push {r0-r1}
    swi #6
    bx lr

waitpid:
    push {r0-r1}
    swi #7
    bx lr

fork_exec:
    push {r0-r1}
    swi #8
    bx lr

get_time:
    swi #9
    bx lr

exec:
    push {r0-r1}
    swi #10
    bx lr

open:
    push {r0-r1}
    swi #11
    bx lr

close:
    push {r0}
    swi #12
    bx lr

read:
    push {r0-r2}
    swi #13
    bx lr

write:
    push {r0-r2}
    swi #14
    bx lr

remove:
    push {r0}
    swi #15
    bx lr

readdir:
    push {r0}
    swi #16
    bx lr

seek:
    push {r0-r2}
    swi #17
    bx lr

ioctl:
    push {r0-r2}
    swi #18
    bx lr

stat:
    push {r0-r1}
    swi #19
    bx lr

mount:
    push {r0-r2}
    swi #20
    bx lr

unmount:
    push {r0}
    swi #21
    bx lr

chmod:
    push {r0-r1}
    swi #22
    bx lr

insmod:
    push {r0-r2}
    swi #23
    bx lr
