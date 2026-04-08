#ifndef VFS_H
# define VFS_H

# include <stdint.h>

# include "fs/devfs.h"
# include "fs/fat32.h"
# include "fs/vffs.h"
# include "mem.h"

// Minimalist VFS supporting multiple filesystem types.
// No directory management: only mounting points as top-level
// directories, each containing flat files.
// All paths follow the format "/<mountpoint>/<file>".

# define NBMAX_MOUNTINGPOINT 4096
# define NBMAX_FD            4096

# define O_CREAT             0x000F
# define O_APPEND            0x00F0
# define O_RDWR              0x0F00
# define O_RDONLY            0xF000

# define SEEK_SET            0
# define SEEK_CUR            1
# define SEEK_END            2

typedef enum
{
    DEVICES,
    VFILES,
    FAT32,
    // Add new filesystem types here.
} e_fstype;

typedef struct mounting_point
{
    const char *name;
    e_fstype type;
    void *fs;
} s_mpoint;

typedef struct vfs
{
    s_mpoint list[NBMAX_MOUNTINGPOINT];
    uint32_t nbmountpoints;
} s_vfs;

extern s_vfs vfsroot;

typedef struct fd
{
    e_fstype type;
    int mode;
    void *addr;
    uint32_t offset;
} s_fd;

typedef struct stat
{
    uint32_t st_size;
    // Add other stat fields here as needed.
} s_stat;

int open(const char *path, int mode);
int close(int fd);

int read(int fd, void *buf, uint32_t len);
int write(int fd, const void *buf, uint32_t len);

int remove(const char *path);
const char **readdir(const char *path);

int seek(int fd, uint32_t offset, int whence);
int ioctl(int fd, int cmd, int args);

int stat(int fd, s_stat *stat);

int mount(const char *devpath, const char *mountpath, e_fstype type);
int unmount(const char *path);

int chmod(const char *path, int mode);
int insmod(const char *path, void *addr, s_driver *driver);

void print_vfs(void);

int init_vfs(void);
int mount_devices(void);

#endif /* !VFS_H */
