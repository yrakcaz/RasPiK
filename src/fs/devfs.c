#include "fs/devfs.h"

int add_device(s_devfs *devfs, const char *name, void *addr, s_driver *driver)
{
    s_device *device = kmalloc(sizeof (s_device));
    if (!device)
        return -1;

    device->name = name;
    device->addr = addr;
    device->driver = driver;
    device->perm = PERM_READ | PERM_WRITE;
    device->lock = 0;

    if (devfs->nbdevice >= NBMAX_DEVICE - 1)
    {
        kfree(driver);
        kfree(device);
        return -1;
    }

    devfs->list[devfs->nbdevice++] = device;
    return device->driver->init(device);
}

int remove_device(s_devfs *devfs, const char *name)
{
    for (int i = 0; i < devfs->nbdevice; i++)
    {
        if (!strcmp(devfs->list[i]->name, name))
        {
            kfree(devfs->list[i]->driver);
            kfree(devfs->list[i]);
            devfs->nbdevice--;
            for (int j = i; j < devfs->nbdevice; j++)
                devfs->list[j] = devfs->list[j + 1];
            return 0;
        }
    }
    return -1;
}

s_device *getnode_devfs(s_devfs *devfs, const char *name)
{
    for (int i = 0; i < devfs->nbdevice; i++)
        if (!strcmp(devfs->list[i]->name, name))
            return devfs->list[i];
    return NULL;
}

int chmod_device(s_devfs *devfs, const char *name, int perm)
{
    for (int i = 0; i < devfs->nbdevice; i++)
    {
        if (!strcmp(devfs->list[i]->name, name))
        {
            devfs->list[i]->perm = perm;
            return 0;
        }
    }
    return -1;
}

const char **readdir_devfs(s_devfs *devfs)
{
    const char **ret = kmalloc(devfs->nbdevice + 1 * sizeof (const char *));
    if (!ret)
        return NULL;

    int i;
    for (i = 0; i < devfs->nbdevice; i++)
        ret[i] = devfs->list[i]->name;
    ret[i] = NULL;

    return ret;
}

s_devfs *create_devfs(void)
{
    s_devfs *devfs = kmalloc(sizeof (s_devfs));
    if (!devfs)
        return NULL;

    devfs->nbdevice = 0;
    for (int i = 0; i < NBMAX_DEVICE; i++)
        devfs->list[i] = NULL;

    return devfs;
}

void remove_devfs(s_devfs *devfs)
{
    for (int i = 0; i < NBMAX_DEVICE; i++)
    {
        if (devfs->list[i])
        {
            kfree(devfs->list[i]->driver);
            kfree(devfs->list[i]);
        }
    }
    kfree(devfs);
}
