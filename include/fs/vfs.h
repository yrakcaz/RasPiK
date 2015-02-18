#ifndef VFS_H
# define VFS_H

# include <stdint.h>
# include "mem.h"
# include "fs/vffs.h"
# include "fs/devfs.h"
# include "fs/fat32.h"

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
# define FAT32               2
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
    int mode;
    void *addr;
    uint32_t offset;
} s_fd;

typedef struct stat
{
    uint32_t st_size;
    //TODO : ADD TIME!
} s_stat;

int open(const char *path, int mode); //Syscall
int close(int fd); //Syscall

int read(int fd, void *buf, uint32_t len); //Syscall
int write(int fd, const void *buf, uint32_t len); //Syscall

int remove(const char *path); //Syscall
const char **readdir(const char *path); //Syscall

int seek(int fd, uint32_t offset, int whence); //Syscall
int ioctl(int fd, int cmd, int args); //Syscall

int stat(int fd, s_stat *stat); //Syscall

int mount(const char *devpath, const char *mountpath, int type); //Syscall
int unmount(const char *path); //Syscall

int chmod(const char *path, int mode); //Syscall
int insmod(const char *path, void *addr, s_driver *driver); //Syscall

void print_vfs(void);

int init_vfs(void);
int mount_devices(void);

#endif /* !VFS_H */
