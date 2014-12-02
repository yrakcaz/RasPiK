#ifndef IO_H
# define IO_H

# include "vfs.h"

/* Usefull defines. */

# define NBMAX_FD  4096
# define NBMAX_DIR 4096
# define EOF       -1

# define O_RDONLY  0x0000
# define O_RDWR    0x000F
# define O_APPEND  0x00F0
# define O_CREAT   0x0F00

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
void closedir(s_dir *directory);
void print_dir(const char *path); //Just for debugging

int open(const char *name, int mode);
int seek(int fd, uint32_t offset, int whence);
int close(int fd);

int write(int fd, const char *buff, uint32_t size);
int read(int fd, const char *buff, uint32_t size);
int ioctl(int fd, int op, void *args);

int remove(const char *path);

uint32_t get_filesize(int fd); //Get file size?

int init_io(void); //fill initial vfs?

#endif /* !IO_H */
