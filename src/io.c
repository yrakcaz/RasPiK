#include "io.h"

static s_fd *fd_table[NBMAX_FD];
static s_dir *dir_table[NBMAX_DIR];

static const char *pwd = "/";

void chdir(const char *path)
{
    pwd = path;
}

const char *get_pwd(void)
{
    return pwd;
}

static void free_dir(s_dir *dir)
{
    for (int i = 0; i < dir->nbentries; i++)
        kfree(dir->entries[i]);
    kfree(dir->entries);
    kfree(dir);
}

s_dir *opendir(const char *name)
{
    if (!strcmp(name, ""))
        return NULL;

    char *path = name[0] == '/' ? (char *)name : strcat(pwd, name);
    s_vfsdir *vfsdir = getdir(path);
    if (!vfsdir)
        return NULL;

    s_dir *dir = kmalloc(sizeof (s_dir));
    if (!dir)
        return NULL;

    int i;
    for (i = 0; dir_table[i]; i++) {}
    if (i >= NBMAX_DIR)
    {
        kfree(dir);
        return NULL;
    }

    dir->idx = i;
    for (i = strlen(name) - 1; i >= 0 && name[i] != '/'; i--) {}
    dir->name = i == 0 ? name : name + i + 1;
    dir->nbentries = vfsdir->nbinodes;
    dir->entries = kmalloc(dir->nbentries * sizeof (s_direntry *));
    if (!dir->entries)
    {
        kfree(dir);
        return NULL;
    }

    for (i = 0; i < dir->nbentries; i++)
    {
        s_direntry *entry = kmalloc(sizeof (s_direntry));
        if (!entry)
        {
            free_dir(dir);
            return NULL;
        }
        entry->type = vfsdir->list[i]->type;
        entry->name = vfsdir->list[i]->name;
        dir->entries[i] = entry;
    }

    dir_table[dir->idx] = dir;
    return dir;
}

int mkdir(const char *name)
{
    if (!strcmp(name, "") || !strcmp(name, "/"))
        return 0;

    char *path = name[0] == '/' ? (char *)name : strcat(pwd, name);
    s_vfsinode *inode = kmalloc(sizeof (s_vfsinode));
    if (!inode)
        return 0;

    int i;
    inode->inumber = inumber++;
    inode->type = DIR;
    for (i = strlen(path) - 1; path[i] != '/'; i--) {}
    inode->name = path + i + 1;

    s_vfsdir *dir = kmalloc(sizeof (s_vfsdir));
    if (!dir)
    {
        kfree(inode);
        return 0;
    }

    dir->name = inode->name;

    dir->nbinodes = 2;
    dir->list = kmalloc(dir->nbinodes * sizeof (s_vfsinode *));

    s_vfsinode *dot = kmalloc(sizeof (s_vfsinode));
    if (!dot)
    {
        free_vfsinode(inode);
        return 0;
    }
    s_vfsinode *doubledot = kmalloc(sizeof (s_vfsinode));
    if (!doubledot)
    {
        kfree(dot);
        free_vfsinode(inode);
        return 0;
    }

    dot->inumber = inumber++;
    doubledot->inumber = inumber++;
    dot->type = DIR;
    doubledot->type = DIR;
    dot->name = ".";
    doubledot->name = "..";
    dot->node = (void *)dir;

    char *parent = kmalloc(strlen(path));
    for (int k = 0; k < i; k++)
        parent[k] = path[k];
    doubledot->node = getdir(parent);

    dir->list[0] = dot;
    dir->list[1] = doubledot;

    inode->node = (void *)dir;

    return add_vfsentry(path, inode);
}

int init_io(void)
{
    for (int i = 0; i < NBMAX_FD; i++)
        fd_table[i] = NULL;
    for (int i = 0; i < NBMAX_DIR; i++)
        dir_table[i] = NULL;
    if (!mkdir("/dev") || !mkdir("/home") || !mkdir("/etc"))
        return 0;
    if (!mkdir("/home/pi") || !mkdir("/home/root"))
        return 0;
    return 1;
}
