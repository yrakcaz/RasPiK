#include "driver.h"
#include "vfs.h"
#include "drivers/uart.h"

int add_driver(const char *name, void *addr, s_driver *drv)
{
    int ret;
    struct vfsdev *dev = kmalloc(sizeof (struct vfsdev));
    if (!dev)
        return 0;

    dev->name = name;
    dev->addr = addr;
    dev->drv = drv;
    ret = add_deventry(dev);
    if (!ret)
    {
        kfree(dev);
        return ret;
    }

    drv->init(dev);
    return ret;
}

int remove_driver(const char *name)
{
    return remove_vfsentry(name);
}

int init_drivers(void)
{
    if (!init_uart_driver())
        return 0;
    return 1;
}
