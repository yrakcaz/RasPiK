#ifndef UART_H
# define UART_H

# include "mmio.h"
# include "common.h"

/* Usefull defines for UART driver. */
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
void init_uart(void);

/* Puts a char to the UART. */
void putchar_uart(uint8_t byte);

/* Gets a char from the UART. */
uint8_t getchar_uart(void);

/* Writes a string to the UART. */
void write_uart(char *str, unsigned int size);

#endif /* !UART_H */
