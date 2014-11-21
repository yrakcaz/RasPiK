#ifndef COMMON_H
# define COMMON_H

/* Define GPIO addresses. */
# define BASE_GPIO   0x20200000
# define GPPUD       (BASE_GPIO + 0x94)
# define GPPUDCLK0   (BASE_GPIO + 0x98)

/* Define UART0 addresses. */
# define BASE_UART   0x20201000
# define DR_UART     (BASE_UART + 0x00)
# define RSRECR_UART (BASE_UART + 0x04)
# define FR_UART     (BASE_UART + 0x18)
# define ILPR_UART   (BASE_UART + 0x20)
# define IBRD_UART   (BASE_UART + 0x24)
# define FBRD_UART   (BASE_UART + 0x28)
# define LCRH_UART   (BASE_UART + 0x2C)
# define CR_UART     (BASE_UART + 0x30)
# define IFLS_UART   (BASE_UART + 0x34)
# define IMSC_UART   (BASE_UART + 0x38)
# define RIS_UART    (BASE_UART + 0x3C)
# define MIS_UART    (BASE_UART + 0x40)
# define ICR_UART    (BASE_UART + 0x44)
# define DMACR_UART  (BASE_UART + 0x48)
# define ITCR_UART   (BASE_UART + 0x80)
# define ITIP_UART   (BASE_UART + 0x84)
# define ITOP_UART   (BASE_UART + 0x88)
# define TDR_UART    (BASE_UART + 0x8C)

/* Masks */
#define MASK32 0x00000000

/* Loop ncycles processor cycles doing anything. */
static inline void wait(uint32_t ncycles)
{
   asm volatile (
                      "__wait_%=: subs %[ncycles], %[ncycles], #1; bne __wait_%=\n"
                      :: [ncycles]"r"(ncycles)
                      :  "cc"
                     );
}

#endif /* !COMMON_H */
