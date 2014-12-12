#ifndef VFS_H
# define VFS_H

# include <stdint.h>

// The idea is to have the most simple vfs as possible,
// which could handle different fs types, and read/write
// just files in it.
// The solution I have found is to have any directory
// management : just a list of mounting points which
// are fake directories, containing just files.
// So every paths will be like that : "/<dir>/<file>".

# define NBMAX_MOUNTINGPOINT 4096
# define NBMAX_FD            4096

# define DEVICES             0
# define VFILES              1
# define EXT2                2
// [...]

typedef struct mounting_point
{
    const char *name;
    int type;
    void *fs;
} s_mpoint;

typedef struct vfs
{
    s_mpoint list[NBMAX_MOUNTINGPOINT];
    uint32_t nbmpoints;
} s_vfs;

s_vfs vfsroot;

typedef struct fd
{
    int type;
    void *addr;
    uint32_t offset;
} s_fd;

int open(const char *path, int mode);
int close(int fd);

int read(int fd, void *buf, uint32_t len);
int write(int fd, const void *buf, uint32_t len);

int remove(const char *path);
char **readdir(const char *path);

int seek(int fd, uint32_t offset, int whence);
int ioctl(int fd, int cmd, int flags);

int mount(/* DEVICE */void *dev, int type);
int unmount(/* DEVICE */void *dev, int type);

int chmod(const char *path, int mode);

int init_vfs(void);

#endif /* !VFS_H */
