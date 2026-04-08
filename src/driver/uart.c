#include "driver/uart.h"

static int init_uart(s_device *dev)
{
    DO_NOTHING_WITH(dev);

    write_mmio(CR_UART, MASK32);

    write_mmio(GPPUD, MASK32);
    usleep(DELAY);

    write_mmio(GPPUDCLK0, DISABLE_1415);
    usleep(DELAY);

    write_mmio(GPPUDCLK0, MASK32);

    write_mmio(ICR_UART, CLEAR_INTERRUPTS);

    write_mmio(IBRD_UART, UART_DIVIDER);
    write_mmio(FBRD_UART, UART_FRAC_PART_REG);

    write_mmio(LCRH_UART, UART_PERM);

    write_mmio(IMSC_UART, UART_MSK_INTERRUPTS);

    write_mmio(CR_UART, ENABLE_TRANSFER);

    return 0;
}

void putchar_uart(uint8_t byte)
{
    while (read_mmio(FR_UART) & WRITE_OK);
    write_mmio(DR_UART, byte);
}

static uint8_t getchar_uart(void)
{
    while (read_mmio(FR_UART) & READ_OK);
    return read_mmio(DR_UART);
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

int init_uart_driver(void)
{
    s_driver *driver = kmalloc(sizeof (s_driver));
    if (!driver)
        return 0;

    driver->init = init_uart;
    driver->write = write_uart;
    driver->read = read_uart;
    driver->ioctl = ioctl_noop;

    int ret = insmod("/dev/uart", (void *)BASE_UART, driver);

    if (ret < 0)
        klog_ko("UART driver initialization failed");
    else
        klog_ok("UART driver initialized");

    return ret;
}
