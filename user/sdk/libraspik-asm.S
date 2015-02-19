.global test
.global sbrk
.global open
.global write
.global readdir

test:
    push {r0}
    swi #0
    bx lr

sbrk:
    push {r0}
    swi #1
    pop {r0}

open:
    push {r0-r1}
    swi #1
    pop {r0}

write:
    push {r0-r2}
    swi #14
    pop {r0}

readdir:
    push {r0}
    swi #16
    pop {r0}
