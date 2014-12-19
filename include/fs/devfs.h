#ifndef DEVFS_H
# define DEVFS_H

# include "mem.h"

# define NBMAX_DEVICE 4096

# define PERM_READ    0x0F
# define PERM_WRITE   0xF0

struct device;

typedef struct driver
{
    int (*init) (struct device *);
    int (*read) (struct device *, uint32_t *, void *, uint32_t);
    int (*write) (struct device *, uint32_t *, const void *, uint32_t);
    int (*ioctl) (struct device *, int, int);
} s_driver;

typedef struct device
{
    const char *name;
    void *addr;
    s_driver *driver;
    int perm;
    int lock;
} s_device;

typedef struct devfs
{
    s_device *list[NBMAX_DEVICE];
    int nbdevice;
} s_devfs;

int add_device(s_devfs *devfs, const char *name, void *addr, s_driver *driver);
int remove_device(s_devfs *devfs, const char *name);
s_device *getnode_devfs(s_devfs *devfs, const char *name);
int chmod_device(s_devfs *devfs, const char *name, int perm);
const char **readdir_devfs(s_devfs *devfs);
s_devfs *create_devfs(void);
void remove_devfs(s_devfs *devfs);

#endif /* !DEVFS_H */
