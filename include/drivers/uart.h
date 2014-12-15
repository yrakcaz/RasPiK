#ifndef UART_H
# define UART_H

# include "mmio.h"
# include "utils.h"
# include "fs/vfs.h"
# include "timers.h"

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

/* Usefull defines for UART driver. */
# define DISABLE_1415        (1 << 14) | (1 << 15)
# define CLEAR_INTERRUPTS    0x7FF
# define UART_DIVIDER        1
# define UART_FRAC_PART_REG  40
# define UART_RIGHTS         (1 << 4) | (1 << 5) | (1 << 6)
# define UART_MSK_INTERRUPTS (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | \
                             (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10)
# define ENABLE_TRANSFERT    (1 << 1) | (1 << 8) | (1 << 9)
# define WRITE_OK            (1 << 5)
# define READ_OK             (1 << 4)
# define DELAY               150

/* UART initialization function. */
int init_uart_driver(void);

#endif /* !UART_H */
