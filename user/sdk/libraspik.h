#ifndef LIB_RASPIK
# define LIB_RASPIK

# include <stdint.h>

# define BLACK  0x000000
# define WHITE  0xFFFFFF
# define RED    0xFF0000
# define GREEN  0x00FF00
# define BLUE   0x0000FF
# define YELLOW 0xFFFF00

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

//TODO : ADD INTERSTING MACCROS AND STRUCTURES...

typedef struct stat
{
    uint32_t st_size;
    //TODO : ADD TIME!
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

extern int klog(const char *str, uint32_t size, int color);
extern void *sbrk(uint32_t increment);
extern void outb(uint32_t reg, uint32_t val);
extern uint32_t inb(uint32_t reg);
extern int kill(int pid, int status);
extern void exit(int status);
extern int fork_call(uint32_t addr, char **args);
extern int waitpid(int pid, int *retval);
extern int fork_exec(const char *path, char **args);
extern uint64_t get_time(void);
extern int exec(const char *path, char **args);
extern int open(const char *path, int mode);
extern int close(int fd);
extern int read(int fd, void *buf, uint32_t len);
extern int write(int fd, const void *buf, uint32_t len);
extern int remove(const char *path);
extern const char **readdir(const char *path);
extern int seek(int fd, uint32_t offset, int whence);
extern int ioctl(int fd, int cmd, int args);
extern int stat(int fd, s_stat *stat);
extern int mount(const char *devpath, const char *mountpath, int type);
extern int unmount(const char *path);
extern int chmod(const char *path, int mode);
extern int insmod(const char *path, void *addr, s_driver *driver);

#endif /* !LIB_RASPIK */
