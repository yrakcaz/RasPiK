#ifndef IO_H
# define IO_H

# include "vfs.h"

/* File descriptor structure. */
typedef struct file_descriptor
{
    s_vfsinode *inode;
    uint32_t offset;
} s_fd;

/* Directory user shared structure. */
typedef struct dir
{
    int idx;
    const char *name;
    const char **files;
    int nbfiles;
    const char **dirs;
    int nbdirs;
} s_dir;

s_dir *opendir(const char *name);
int createdir(const char *name);
int closedir(s_dir directory);

void chdir(const char *path);                                   //DONE
const char *get_pwd(void);                                      //DONE

int open(const char *name, int mode);
int seek(int fd, uint32_t offset, int whence);
int close(int fd);

int write(int fd, const char *buff, uint32_t size);
int read(int fd, const char *buff, uint32_t size);
int ioctl(int fd, int op, void *args);

uint32_t stat(int fd); //Get file size?

int init_io(void); //fill initial vfs?

#endif /* !IO_H */
