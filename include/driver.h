#ifndef DRIVER_H
# define DRIVER_H

struct vfsdev;

typedef struct driver
{
    const char *name;
    void (*init) (struct vfsdev *);
    int (*read) (struct vfsdev *, void *, uint32_t);
    int (*write) (struct vfsdev *, const void *, uint32_t);
    int (*ioctl) (struct vfsdev *, int, void *);
} s_driver;

int add_driver(const char *name, void *addr, s_driver *drv);
int remove_driver(const char *name);

#endif /* !DRIVER_H */
