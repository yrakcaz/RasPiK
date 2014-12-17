#include "drivers/uart.h"

void init_uart(s_device *dev)
{
    DO_NOTHING_WITH(dev);

    // Disabling UART
    write_mmio(CR_UART, MASK32);

    // Disabling GPIO
    write_mmio(GPPUD, MASK32);
    wait(DELAY);

    // Disabling pin 14 and 15
    write_mmio(GPPUDCLK0, DISABLE_1415);
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

static void putchar_uart(uint8_t byte)
{
    // Waiting for the device...
    while (1)
        if (!(read_mmio(FR_UART) & WRITE_OK))
            break;
    // Then write data.
    write_mmio(DR_UART, byte);
}

static uint8_t getchar_uart(void)
{
    // Waiting for the device...
    while (1)
        if (!(read_mmio(FR_UART) & READ_OK))
            break;
    // Then read data.
    return read_mmio(DR_UART);
}

int write_uart(s_device *dev, uint32_t *offset, const void *str, uint32_t size)
{
    DO_NOTHING_WITH(offset);
    if (((dev->perm & PERM_WRITE) != PERM_WRITE) || dev->lock)
        return -1;
    dev->lock = 1;
    uint32_t i;
    for (i = 0; i < size; i++)
        putchar_uart(((uint8_t *)str)[i]);
    dev->lock = 0;
    return i;
}

int read_uart(s_device *dev, uint32_t *offset, void *str, uint32_t size)
{
    DO_NOTHING_WITH(dev);
    DO_NOTHING_WITH(offset);
    uint32_t i;
    for (i = 0; i < size; i++)
        ((uint8_t *)str)[i] = getchar_uart();
    return i;
}

int ioctl_uart(s_device *dev, int op, void *args)
{
    DO_NOTHING_WITH(dev);
    DO_NOTHING_WITH(op);
    DO_NOTHING_WITH(args);
    return 0;
}

int init_uart_driver(void)
{
    s_driver *driver = kmalloc(sizeof (s_driver));
    if (!driver)
        return 0;

    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    driver->init = init_uart;
    driver->write = write_uart;
    driver->read = read_uart;
    driver->ioctl = ioctl_uart;

    int ret = insmod("/dev/uart", (void *)BASE_UART, driver);
    wait(HUMAN_TIME / 2);

    if (ret < 0)
    {
        klog("\b\b\b\bKO", 6, RED);
        klog("]\tUART driver initialization failed.\n", 37, WHITE);
    }
    else
    {
        klog("\b\b\b\bOK", 6, GREEN);
        klog("]\tUART driver initialized!\n", 27, WHITE);
    }

    return ret;
}
