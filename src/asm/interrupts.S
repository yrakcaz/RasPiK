.global vector
.global raise
.global end_irq

.extern start
.extern treat_reset
.extern treat_undef
.extern treat_swi
.extern treat_pref_abort
.extern treat_data_abort
.extern treat_unused
.extern treat_irq
.extern treat_fiq

vector:
    // Interrupts vector
    b treat_reset
    b treat_undef
    b swi_handler
    b treat_pref_abort
    b treat_data_abort
    b treat_unused
    b irq_handler
    b treat_fiq

swi_handler:
    // Save and restore context before and after irq handling
    stmfd sp!, {r0,r1,r2,r3,r4,r14}
    bl treat_swi
    ldmfd sp!, {r0,r1,r2,r3,r4,pc}^
    bx lr

irq_handler:
    // Save context before irq handling
    push {r0-r12}
    mov r0, lr
    cps #0x1F
    push {r0}
    cps #0x12
    pop {r0-r12}
    cps #0x1F
    push {r0-r12}
    push {lr}
    mrs r0, SPSR
    push {r0}
    cps #0x12
    mov r0, lr
    cps #0x1F
    bl treat_irq

end_irq:
    pop {r0}
    msr SPSR_cxsf, r0
    pop {lr}
    pop {r0-r12}
    cpsie i
    pop {pc}

treat_reset:
    mov r0, #0x8000
    mov r1, #0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    b start
    bx lr

loop:
    b loop

raise:
    //raise for gcclib's div0
    b treat_undef
