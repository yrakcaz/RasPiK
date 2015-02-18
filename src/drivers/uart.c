#include "drivers/uart.h"

static int init_uart(s_device *dev)
{
    DO_NOTHING_WITH(dev);

    // Disabling UART
    outb(CR_UART, MASK32);

    // Disabling GPIO
    outb(GPPUD, MASK32);
    wait(DELAY);

    // Disabling pin 14 and 15
    outb(GPPUDCLK0, DISABLE_1415);
    wait(DELAY);

    // Make it takin' effect
    outb(GPPUDCLK0, MASK32);

    // Clearing interrupts
    outb(ICR_UART, CLEAR_INTERRUPTS);

    // Setting UART Baud Rate
    outb(IBRD_UART, UART_DIVIDER);
    outb(FBRD_UART, UART_FRAC_PART_REG);

    // Setting UART rights
    outb(LCRH_UART, UART_RIGHTS);

    // Masking all interrupts
    outb(IMSC_UART, UART_MSK_INTERRUPTS);

    // Enabling reading and writing
    outb(CR_UART, ENABLE_TRANSFERT);

    return 0;
}

static void putchar_uart(uint8_t byte)
{
    // Waiting for the device...
    while (1)
        if (!(inb(FR_UART) & WRITE_OK))
            break;
    // Then write data.
    outb(DR_UART, byte);
}

static uint8_t getchar_uart(void)
{
    // Waiting for the device...
    while (1)
        if (!(inb(FR_UART) & READ_OK))
            break;
    // Then read data.
    return inb(DR_UART);
}

static int write_uart(s_device *dev, uint32_t *offset, const void *str, uint32_t size)
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

static int read_uart(s_device *dev, uint32_t *offset, void *str, uint32_t size)
{
    DO_NOTHING_WITH(dev);
    DO_NOTHING_WITH(offset);
    uint32_t i;
    for (i = 0; i < size; i++)
        ((uint8_t *)str)[i] = getchar_uart();
    return i;
}

static int ioctl_uart(s_device *dev, int cmd, int args)
{
    DO_NOTHING_WITH(dev);
    DO_NOTHING_WITH(cmd);
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
