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
    dev->lock = 0;
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
