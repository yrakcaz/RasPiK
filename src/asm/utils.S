.global get_sp
.global st_read
.global st_delay

get_sp:
    str sp, [sp]
    ldr r0, [sp]
    mov pc, lr

st_read:
    ldr  r0, =0x20003000
    ldrd r0, r1, [r0, #4]
    bx lr

st_delay:
    push {r3, r4, r5, lr}
    adds r4, r2, r0
    adc  r5, r3, r1
    ldr  r2, =0x20003000
2:
    ldrd  r0, r1, [r2, #4]
    cmp   r1, r5
    cmpeq r0, r4
    bcc   2b
    pop   {r3, r4, r5, pc}
