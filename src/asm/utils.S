.global get_sp
.global fsel_gpio

get_sp:
    str sp, [sp]
    ldr r0, [sp]
    mov pc, lr

fsel_gpio:
    push {lr}
    mov  r3, r0
    ldr  r0, =0x20200000
1:
    cmp   r3, #9
    subhi r3, #10
    addhi r0, #4
    bhi   1b
    add   r3, r3, lsl #1
    mov   r2, #7
    lsl   r2, r3
    lsl   r1, r3
    ldr   r3, [r0]
    and   r1, r2, r1
    bic   r3, r3, r2
    orr   r3, r3, r1
    str   r3, [r0]
    pop   {pc}
