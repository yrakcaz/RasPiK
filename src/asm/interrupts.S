.global vector
.global raise

.extern treat_reset
.extern treat_undef
.extern treat_swi
.extern treat_pref_abort
.extern treat_data_abort
.extern treat_unused
.extern treat_irq
.extern treat_fiq

vector:
    b treat_reset
    b treat_undef
    b treat_swi
    b treat_pref_abort
    b treat_data_abort
    b treat_unused
    b irq_handler
    b treat_fiq

irq_handler:
    sub r14, r14, #4
    stmfd sp!, {r0,r1,r2,r3,r4,r14}
    bl treat_irq
    ldmfd sp!, {r0,r1,r2,r3,r4,pc}^
    bx lr

raise:
    b treat_undef
