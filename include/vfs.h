#ifndef VFS_H
# define VFS_H

# include "mem.h"
# include "console.h"
# include "utils.h"

/*
** We don't need more that this virtual file system for this bare-metal OS
** until we add external mass storage devices.
*/

/* Usefull defines. */

# define NBMAX_FD  4096
# define NBMAX_DIR 4096

# define DIR       0
# define FILE      1
# define EXEC      2

/* Virtual file system structures */

typedef struct vfsinode
{
    int inumber;
    int type;
    const char *name;
    void *node;
} s_vfsinode;

typedef struct vfsfile
{
    const char *name;
    char *data;
} s_vfsfile;

typedef struct vfsdir
{
    const char *name;
    int nbinodes;
    s_vfsinode **list;
} s_vfsdir;

typedef struct vfsexec
{
    const char *name;
    void (*addr)(void);
} s_vfsexec;

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

/* External functions */
int init_vfs(void);                                           //DONE
int add_vfsentry(const char *path, int type, void *addr);
int remove_vfsentry(const char *path);

/* Syscalls */
s_dir *opendir(const char *name);
int createdir(const char *name);
int closedir(s_dir directory);

void chdir(const char *path);                                 //DONE
const char *get_pwd(void);                                    //DONE

int open(const char *name, int mode);
int write(int fd, const char *buff, uint32_t size);
int read(int fd, const char *buff, uint32_t size);
int seek(int fd, uint32_t offset, int whence);
int close(int fd);

void print_vfs(void);                                         //DONE

#endif /* !VFS_H */
