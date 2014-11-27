.global syscall_handler

syscall_handler:
    blx r0
    bx lr
