.section ".text.boot"
.globl start

start:
    //Set up registers
    mov sp, #0x8000
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
    blx r3

halt:
    //Then halt!
    wfe
    b halt
