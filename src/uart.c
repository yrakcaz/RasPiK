#include "../include/uart.h"

void init_uart(void)
{
    // Disabling UART
    write_mmio(CR_UART, MASK32);

    // Disabling GPIO
    write_mmio(GPPUD, MASK32);
    wait(DELAY);

    // Disabling pin 14 and 15
    write_mmio(GPPUDCLK0, (1 << 14) | (1 << 15));
    wait(DELAY);

    // Make it takin' effect
    write_mmio(GPPUDCLK0, MASK32);

    // Clearing interrupts
    write_mmio(ICR_UART, CLEAR_INTERRUPTS);

    // Setting UART Baud Rate
    write_mmio(IBRD_UART, UART_DIVIDER);
    write_mmio(FBRD_UART, UART_FRAC_PART_REG);

    // Setting UART rights
    write_mmio(LCRH_UART, UART_RIGHTS);

    // Masking all interrupts
    write_mmio(IMSC_UART, UART_MSK_INTERRUPTS);

    // Enabling reading and writing
    write_mmio(CR_UART, ENABLE_TRANSFERT);
}

void putchar_uart(uint8_t byte)
{
    // Waiting for the device...
    while (1)
        if (!(read_mmio(FR_UART) & WRITE_OK))
            break;
    // Then write data.
    write_mmio(DR_UART, byte);
}

uint8_t getchar_uart(void)
{
    // Waiting for the device...
    while (1)
        if (!(read_mmio(FR_UART) & READ_OK))
            break;
    // Then read data.
    return read_mmio(DR_UART);
}

void write_uart(char *str, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        putchar_uart(str[i]);
}
