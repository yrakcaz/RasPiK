.global get_sp

get_sp:
    str sp, [sp]
    ldr r0, [sp]
    mov pc, lr
