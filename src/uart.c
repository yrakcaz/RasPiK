#include "../include/uart.h"

void init_uart(void)
{
    //COMMENT AND MAKE MACCROS!!

    write_mmio(CR_UART, 0x00000000);

    write_mmio(GPPUD, 0x00000000);
    wait(150);

    write_mmio(GPPUDCLK0, (1 << 14) | (1 << 15));
    wait(150);

    write_mmio(GPPUDCLK0, 0x00000000);

    write_mmio(ICR_UART, 0x7FF);

    write_mmio(IBRD_UART, 1);
    write_mmio(FBRD_UART, 40);

    write_mmio(LCRH_UART, (1 << 4) | (1 << 5) | (1 << 6));

    write_mmio(IMSC_UART, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                          (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    write_mmio(CR_UART, (1 << 1) | (1 << 8) | (1 << 9));
}

void putchar_uart(uint8_t byte)
{
    while (1)
        if (!(read_mmio(FR_UART) & (1 << 5)))
            break;
    write_mmio(DR_UART, byte);
}

uint8_t getchar_uart(void)
{
    while (1)
        if (!(read_mmio(FR_UART) & (1 << 4)))
            break;
    return read_mmio(DR_UART);
}

void putstring_uart(const char *str)
{
    for (int i = 0; str[i]; i++)
        putchar_uart(str[i]);
}
