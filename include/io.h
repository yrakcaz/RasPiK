#ifndef IO_H
# define IO_H

# include "vfs.h"

/* Usefull defines. */

# define NBMAX_FD  4096
# define NBMAX_DIR 4096
# define EOF       0xFF

# define O_RDONLY  0x0000
# define O_RDWR    0x000F
# define O_APPEND  0x00F0
# define O_CREAT   0x0F00

# define SEEK_SET 0
# define SEEK_CUR 1
# define SEEK_END 2

/* File descriptor structure. */
typedef struct file_descriptor
{
    s_vfsinode *inode;
    uint32_t offset;
    int flags;
} s_fd;

/* Directory user shared structure. */
typedef struct direntry
{
    int type;
    const char *name;
} s_direntry;

typedef struct dir
{
    int idx;
    const char *name;
    int nbentries;
    s_direntry **entries;
} s_dir;

void chdir(const char *path);
const char *get_pwd(void);

s_dir *opendir(const char *name);
int mkdir(const char *name);
int closedir(s_dir *directory);

int open(const char *name, int mode);
int seek(int fd, uint32_t offset, int whence);
int close(int fd);

int write(int fd, const void *buff, uint32_t size);
int read(int fd, void *buff, uint32_t size);
int ioctl(int fd, int op, void *args);

int remove(const char *path);

uint32_t get_filesize(int fd);

int init_io(void);

#endif /* !IO_H */
