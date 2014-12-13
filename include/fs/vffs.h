#ifndef VFFS_H
# define VFFS_H

# include "mem.h"

# define NBMAX_FILE 4096

# define EOF        0xFF

# define PERM_READ  0x0F
# define PERM_WRITE 0xF0

typedef struct vfile
{
    const char *name;
    uint32_t size;
    char *data;
    int perm;
    int lock;
} s_vfile;

typedef struct vfilefs
{
    s_vfile *list[NBMAX_FILE];
    int nbfile;
} s_vffs;

int create_vfile(s_vffs *vffs, const char *name);
s_vfile *getnode_vffs(s_vffs *vffs, const char *name);
int remove_vfile(s_vffs *vffs, const char *name);
int read_vfile(s_vfile *file, uint32_t offset, void *buf, uint32_t len);
int write_vfile(s_vfile *file, uint32_t offset, const void *buf, uint32_t len);
int chmod_vfile(s_vffs *vffs, const char *name, int perm);
const char **readdir_vffs(s_vffs *vffs);
s_vffs *create_vffs(void);
void remove_vffs(s_vffs *vffs);

#endif /* !VFFS_H */
