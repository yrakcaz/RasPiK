.global switch_context
.global context_exit

switch_context:
    b switch_context

context_exit:
    pop {r0, lr}
    add sp, sp, r0
    pop {r0-r12}
    rfeia sp!
