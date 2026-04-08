#ifndef RASPIK_H
# define RASPIK_H

// This header mirrors the kernel ABI exposed to user space.
// It must be kept in sync with the corresponding kernel headers manually.
// Long-term, these definitions should be unified under include/uapi/.

# include <stdint.h>

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
} e_fstype;

typedef struct stat
{
    uint32_t st_size;
} s_stat;

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

extern int brk(void *addr);
extern int clone(uint32_t addr, char **args);
extern int spawn(const char *path, char **args);
extern int exec(const char *path, char **args);
extern void exit(int status);
extern int kill(int pid, int status);
extern int getpid(void);
extern int waitpid(int pid, int *retval);
extern uint32_t gettime(void);
extern int open(const char *path, int mode);
extern int close(int fd);
extern int read(int fd, void *buf, uint32_t len);
extern int write(int fd, const void *buf, uint32_t len);
extern int seek(int fd, uint32_t offset, int whence);
extern int stat(int fd, s_stat *stat);
extern int remove(const char *path);
extern const char **readdir(const char *path);
extern int ioctl(int fd, int cmd, int args);
extern int insmod(const char *path, void *addr, s_driver *driver);
extern int mount(const char *devpath, const char *mountpath, e_fstype type);
extern int unmount(const char *path);
extern int chmod(const char *path, int mode);

#endif /* !RASPIK_H */
