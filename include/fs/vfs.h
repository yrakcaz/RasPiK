#ifndef VFS_H
# define VFS_H

# include <stdint.h>
# include "mem.h"

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

# define O_CREAT             0x000F
# define O_APPEND            0x00F0
# define O_RDWR              0x0F00
# define O_RDONLY            0xF000

# define SEEK_SET            0
# define SEEK_CUR            1
# define SEEK_END            2

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
const char **readdir(const char *path);

int seek(int fd, uint32_t offset, int whence);
int ioctl(int fd, int cmd, int flags);

int mount(const char *path, int type);
int unmount(const char *path);

int chmod(const char *path, int mode);

void print_vfs(void);

int init_vfs(void);

#endif /* !VFS_H */
