#include "driver.h"
#include "vfs.h"

int add_driver(const char *name, void *addr, s_driver *drv)
{
    struct vfsdev *dev = kmalloc(sizeof (struct vfsdev));
    dev->name = name;
    dev->addr = addr;
    dev->drv = drv;
    return add_deventry(dev);
}
