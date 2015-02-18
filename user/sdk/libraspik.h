#ifndef LIB_RASPIK
# define LIB_RASPIK

# include <stdint.h>

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

void *sbrk(uint32_t increment);
void outb(uint32_t reg, uint32_t val);
uint32_t inb(uint32_t reg);
int kill(int pid, int status);
void exit(int status);
int fork_call(uint32_t addr, char **args);
int waitpid(int pid, int *retval);
int fork_exec(const char *path, char **args);
uint64_t get_time(void);
int exec(const char *path, char **args);
int open(const char *path, int mode);
int close(int fd);
int read(int fd, void *buf, uint32_t len);
int write(int fd, const void *buf, uint32_t len);
int remove(const char *path);
const char **readdir(const char *path);
int seek(int fd, uint32_t offset, int whence);
int ioctl(int fd, int cmd, int args);
int stat(int fd, s_stat *stat);
int mount(const char *devpath, const char *mountpath, int type);
int unmount(const char *path);
int chmod(const char *path, int mode);
int insmod(const char *path, void *addr, s_driver *driver);

#endif /* !LIB_RASPIK */
