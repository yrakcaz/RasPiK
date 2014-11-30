#ifndef DRIVER_H
# define DRIVER_H

struct vfsdev;

typedef struct driver
{
    const char *name;
    void (*init) (struct vfsdev *);
    int (*read) (struct vfsdev *, char *, uint32_t);
    int (*write) (struct vfsdev *, const char *, uint32_t);
    int (*ioctl) (struct vfsdev *, int, void *);
} s_driver;

int add_driver(const char *name, void *addr, s_driver *drv);

#endif /* !DRIVER_H */
