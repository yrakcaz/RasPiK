#include "libraspik.h"

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


