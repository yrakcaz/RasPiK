.section ".text.boot"
.globl start

start:
    //We just want to ensure that r0, r1 and r2 will not change before calling k_start.
    push {r0, r1, r2}

    mov r4, #0x8000
    cps #0x13
    add sp, r4, #0x2400
    cps #0x17
    add sp, r4, #0x2800
    cps #0x12
    add sp, r4, #0x2C00
    cps #0x1F
    add sp, r4, #0x3C00
    mrc p15, #0, r4, c1, c0, #0
    orr r4, #0x400000
    mcr p15, #0, r4, c1, c0, #0

    ldr sp, =(128 * (1024 * 1024))
    sub sp, sp, #0x4

    //Set up registers
    ldr r4, =_bss_start
    ldr r9, =_bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b 2f

1:
    stmia r4!, {r5 - r8}

2:
    //Go to _bss_end
    cmp r4, r9
    blo 1b

    //Call kernel entry point
    ldr r3, =k_start
    pop {r0, r1, r2}
    blx r3

halt:
    //Then halt!
    wfe
    b halt
