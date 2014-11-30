#include "driver.h"
#include "vfs.h"

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
        return ret;

    drv->init(dev);
    return ret;
}
