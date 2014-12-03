#include "io.h"

static s_fd fd_table[NBMAX_FD];
static s_dir *dir_table[NBMAX_DIR];

static const char *pwd = "/";

void chdir(const char *path)
{
    pwd = path;
    if (path[strlen(path) - 1] != '/')
        pwd = strcat(pwd, "/");
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
    for (i = 0; dir_table[i] && i < NBMAX_DIR; i++) {}
    if (i >= NBMAX_DIR)
    {
        kfree(dir);
        return NULL;
    }

    dir->idx = i;
    dir->name = path;
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

int closedir(s_dir *directory)
{
    if (!directory || directory->idx >= NBMAX_DIR || !dir_table[directory->idx])
        return 0;
    dir_table[directory->idx] = NULL;
    for (int i = 0; i < directory->nbentries; i++)
        kfree(directory->entries[i]);
    kfree(directory->entries);
    kfree(directory);
    return 1;
}

static s_vfsinode *create_file(const char *path)
{
    s_vfsinode *inode = kmalloc(sizeof (s_vfsinode));
    if (!inode)
        return NULL;
    inode->inumber = inumber++;
    inode->type = FILE;
    int i;
    for (i = strlen(path) - 1; path[i] != '/'; i--) {}
    inode->name = path + i + 1;

    s_vfsfile *file = kmalloc(sizeof (s_vfsfile));
    if (!file)
    {
        kfree(inode);
        return NULL;
    }
    file->name = inode->name;
    file->size = 0;
    file->data = kmalloc(sizeof (char));
    if (!file->data)
    {
        kfree(inode);
        kfree(file);
        return NULL;
    }
    file->data[0] = EOF;

    inode->node = (void *)file;
    int result = add_vfsentry(path, inode);
    if (!result)
    {
        free_vfsinode(inode);
        return NULL;
    }
    return inode;
}

int open(const char *name, int mode)
{
    char *path = name[0] == '/' ? (char *)name : strcat(pwd, name);
    s_fd fd;
    fd.inode = get_vfsinode(path);
    if (!fd.inode)
    {
        if ((mode & O_CREAT) == O_CREAT)
            fd.inode = create_file(path);
        if (!fd.inode)
            return -1;
    }
    if ((fd.inode->type != FILE) && (fd.inode->type != DEV))
    {
        fd.inode = NULL;
        return -1;
    }
    fd.offset = 0;
    if (((mode & O_APPEND) == O_APPEND) && fd.inode->type == FILE)
        fd.offset = ((s_vfsfile *)fd.inode->node)->size;
    fd.flags = mode;
    int i;
    for (i = 0; fd_table[i].inode && i < NBMAX_FD; i++) {}
    if (i >= NBMAX_FD)
    {
        fd.inode = NULL;
        return -1;
    }
    fd_table[i] = fd;
    return i;
}

int seek(int fd, uint32_t offset, int whence)
{
    if (fd >= NBMAX_FD || !fd_table[fd].inode)
        return -1;
    switch (whence)
    {
        case SEEK_SET:
            fd_table[fd].offset = offset;
            break;
        case SEEK_CUR:
            fd_table[fd].offset += offset;
            break;
        case SEEK_END:
            fd_table[fd].offset = ((s_vfsfile *)fd_table[fd].inode->node)->size + offset;
            break;
        default:
            return -1;
    }
    return fd_table[fd].offset;
}

int close(int fd)
{
    if (fd >= NBMAX_FD || fd_table[fd].inode == NULL)
        return 0;
    fd_table[fd].inode = NULL;
    return 1;
}

int write(int fd, const void *buff, uint32_t size)
{
    if (fd >= NBMAX_FD || !fd_table[fd].inode)
        return -1;
    if ((fd_table[fd].flags & O_RDWR) != O_RDWR)
        return -1;
    if (fd_table[fd].inode->type == FILE)
    {
        s_vfsfile *file = (s_vfsfile *)fd_table[fd].inode->node;
        file->data = krealloc(file->data, file->size + size + 1);
        int i;
        for (i = 0; i < size; i++)
            file->data[fd_table[fd].offset++] = ((char *)buff)[i];
        file->data[fd_table[fd].offset] = EOF;
        file->size += i;
        return i;
    }
    else if (fd_table[fd].inode->type == DEV)
    {
        s_vfsdev *dev = ((s_vfsdev *)fd_table[fd].inode->node);
        return dev->drv->write(dev, buff, size);
    }
    else
        return -1;
}

int read(int fd, void *buff, uint32_t size)
{
    if (fd >= NBMAX_FD || !fd_table[fd].inode)
        return -1;
    if (fd_table[fd].inode->type == FILE)
    {
        s_vfsfile *file = (s_vfsfile *)fd_table[fd].inode->node;
        int i;
        for (i = 0; i < size ; i++)
        {
            ((char *)buff)[i] = file->data[fd_table[fd].offset++];
            if (((char *)buff)[i] == EOF)
                return i;
        }
        return i;
    }
    else if (fd_table[fd].inode->type == DEV)
    {
        s_vfsdev *dev = ((s_vfsdev *)fd_table[fd].inode->node);
        return dev->drv->read(dev, buff, size);
    }
    else
        return -1;

}

int ioctl(int fd, int op, void *args)
{
    if (fd >= NBMAX_FD || !fd_table[fd].inode)
        return -1;
    if (fd_table[fd].inode->type != DEV)
        return -1;
    if ((fd_table[fd].flags & O_RDWR) != O_RDWR) //VERIFY IT
        return -1;
    s_vfsdev *dev = ((s_vfsdev *)fd_table[fd].inode->node);
    return dev->drv->ioctl(dev, op, args);
}

int remove(const char *path)
{
    return remove_vfsentry(path);
}

uint32_t get_filesize(int fd)
{
    if (fd >= NBMAX_FD || !fd_table[fd].inode)
        return -1;
    return ((s_vfsfile *)fd_table[fd].inode->node)->size;
}

int init_io(void)
{
    for (int i = 0; i < NBMAX_FD; i++)
        fd_table[i].inode = NULL;
    for (int i = 0; i < NBMAX_DIR; i++)
        dir_table[i] = NULL;
    if (!mkdir("/dev") || !mkdir("/home"))
        return 0;

    return 1;
}
