#include "driver/console.h"
#include "fs/vfs.h"
#include "klog.h"
#include "utils.h"

static int write_console(s_device *dev, uint32_t *offset, const void *buf, uint32_t len)
{
    DO_NOTHING_WITH(dev);
    DO_NOTHING_WITH(offset);
    return klog_write(buf, len, WHITE);
}

int init_console_driver(void)
{
    s_driver *driver = kmalloc(sizeof(s_driver));
    if (!driver)
        return 0;

    driver->init = init_noop;
    driver->read = read_noop;
    driver->write = write_console;
    driver->ioctl = ioctl_noop;

    int ret = insmod("/dev/console", NULL, driver);

    if (ret < 0)
        klog_ko("Console driver initialization failed");
    else
        klog_ok("Console driver initialized");

    return ret;
}
