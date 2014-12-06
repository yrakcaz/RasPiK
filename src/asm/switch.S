.global context_exit

context_exit:
    pop {r0, lr}
    add sp, sp, r0
    pop {r0-r12}
    rfeia sp!
