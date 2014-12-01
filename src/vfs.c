#include "vfs.h"

int init_vfs(void)
{
    inumber = 0;

    vfsroot.inumber = inumber++;
    vfsroot.type = DIR;
    vfsroot.name = "/";

    s_vfsdir *rootdir = kmalloc(sizeof (s_vfsdir));
    if (!rootdir)
        return 0;

    rootdir->name = "/";
    rootdir->nbinodes = 2;
    rootdir->list = kmalloc(rootdir->nbinodes * sizeof (s_vfsinode *));
    if (!rootdir->list)
    {
        kfree(rootdir);
        return 0;
    }

    vfsroot.node = (void *)rootdir;

    s_vfsinode *dot = kmalloc(sizeof (s_vfsinode));
    if (!dot)
    {
        kfree(rootdir->list);
        kfree(rootdir);
        return 0;
    }
    s_vfsinode *doubledot = kmalloc(sizeof (s_vfsinode));
    if (!doubledot)
    {
        kfree(dot);
        kfree(rootdir->list);
        kfree(rootdir);
        return 0;
    }

    dot->inumber = inumber++;
    doubledot->inumber = inumber++;
    dot->type = DIR;
    doubledot->type = DIR;
    dot->name = ".";
    doubledot->name = "..";
    dot->node = (void *)rootdir;
    doubledot->node = (void *)rootdir;

    rootdir->list[0] = dot;
    rootdir->list[1] = doubledot;

    return 1;
}

static s_vfsdir *getdir_rec(char *path, s_vfsdir *dir)
{
    char *name = kmalloc(strlen(path));
    int i;
    for (i = 0; path[i] != '/' && path[i] != '\0'; i++)
        name[i] = path[i];
    name[i] = '\0';
    for (int j = 0; j < dir->nbinodes; j++)
    {
        if (strcmp(dir->list[j]->name, name))
        {
            kfree(name);
            switch (path[i])
            {
                case '\0':
                    return ((s_vfsdir *)dir->list[j]->node);
                case '/':
                    return getdir_rec(path + i + 1, (s_vfsdir *)dir->list[j]->node);
                default:
                    return NULL;
            }
        }
    }
    kfree(name);
    return NULL;
}

s_vfsdir *getdir(char *path)
{
    if (path[0] != '/' && strlen(path) > 0)
        return NULL;
    s_vfsdir *rootdir = (s_vfsdir *)vfsroot.node;
    if (strlen(path) == 0)
        return rootdir;
    s_vfsdir *ret = getdir_rec(path + 1, rootdir);
    kfree(path);
    return ret;
}

void free_vfsinode(s_vfsinode *inode)
{
    if (!inode)
        return;
    switch (inode->type)
    {
        case FILE:
            kfree(((s_vfsfile *)inode->node)->data);
            break;
        case DIR:
            for (int i = 0; i < ((s_vfsdir *)inode->node)->nbinodes; i++)
                free_vfsinode(((s_vfsdir *)inode->node)->list[i]);
            kfree(((s_vfsdir *)inode->node)->list);
            break;
        case EXEC:
            break;
        case DEV:
            kfree(((s_vfsdev *)inode->node)->drv);
            break;
        default:
            break;
    }
    kfree(inode->node);
    kfree(inode);
}

int add_vfsentry(const char *path, s_vfsinode *inode)
{
    char *dirpath = kmalloc(strlen(path));
    int i, k;
    for (i = strlen(path) - 1; path[i] != '/'; i--) {}
    for (k = 0; k < i; k++)
        dirpath[k] = path[k];
    dirpath[k] = '\0';
    s_vfsdir *dir = getdir(dirpath);
    if (!dir)
    {
        free_vfsinode(inode);
        return 0;
    }
    for (i = 0; i < dir->nbinodes; i++)
    {
        if (!strcmp(dir->list[i]->name, inode->name)) //File already exists;
        {
            kfree(inode);
            return 0;
        }
    }
    dir->nbinodes++;
    dir->list = krealloc(dir->list, dir->nbinodes * sizeof (s_vfsinode *));
    dir->list[dir->nbinodes - 1] = inode;
    return 1;
}

int remove_vfsentry(const char *path)
{
    char *dirpath = kmalloc(strlen(path));
    int i, k;
    for (i = strlen(path) - 1; path[i] != '/'; i--) {}
    for (k = 0; k < i; k++)
        dirpath[k] = path[k];
    dirpath[k] = '\0';
    s_vfsdir *dir = getdir(dirpath);
    if (!dir)
        return 0;
    for (k = 0; k < dir->nbinodes; k++)
    {
        if (!strcmp(path + i + 1, dir->list[i]->name))
        {
            free_vfsinode(dir->list[i]);
            for (int j = i; j < dir->nbinodes - 1; j++)
                dir->list[j] = dir->list[j + 1];
            dir->nbinodes--;
            return 1;
        }
    }
    return 0;
}

int add_execentry(const char *path, int (*addr)(int, char **))
{
    if (!addr)
        return 0;
    s_vfsinode *inode = kmalloc(sizeof (s_vfsinode));
    if (!inode)
        return 0;
    inode->inumber = inumber++;
    inode->type = EXEC;
    int i;
    for (i = strlen(path) - 1; path[i] != '/'; i--) {}
    inode->name = path + i + 1;

    s_vfsexec *exec = kmalloc(sizeof (s_vfsexec));
    if (!exec)
    {
        kfree(inode);
        return 0;
    }
    exec->name = path + i + 1;
    exec->addr = addr;

    inode->node = (void *)exec;
    return add_vfsentry(path, inode);
}

int add_deventry(s_vfsdev *dev)
{
    if (!dev)
        return 0;
    s_vfsinode *inode = kmalloc(sizeof (s_vfsinode));
    if (!inode)
        return 0;
    inode->inumber = inumber++;
    inode->type = DEV;
    inode->name = dev->name;
    inode->node = (void *)dev;
    return add_vfsentry(strcat("/dev/", dev->name), inode);
}

static void print_vfsfile(s_vfsfile *file)
{
    write_console(file->name, strlen(file->name), WHITE);
    write_console(" ", 1, WHITE);
}

static void print_vfsexec(s_vfsexec *exec)
{
    write_console(exec->name, strlen(exec->name), GREEN);
    write_console(" ", 1, GREEN);
}

static void print_vfsdev(s_vfsdev *dev)
{
    write_console(dev->name, strlen(dev->name), YELLOW);
    write_console(" ", 1, YELLOW);
}

static void print_vfsinode(s_vfsinode *inode, const char *path);

static void print_vfsdir(s_vfsdir *dir, const char *path)
{
    char *name;
    if (dir->name[0] == '/')
        name = strcat(path, dir->name);
    else
    {
        name = strcat(path, "/");
        name = strcat(name, dir->name);
    }
    write_console(name, strlen(name), WHITE);
    write_console(":\n\t", 3, WHITE);
    for (int i = 0; i < dir->nbinodes; i++)
    {
        if ((dir->list)[i]->type == DIR)
        {
            write_console((dir->list)[i]->name, strlen((dir->list)[i]->name), BLUE);
            write_console(" ", 1, BLUE);
        }
        else
            print_vfsinode((dir->list)[i], name);
    }
    write_console("\n\n", 2, WHITE);
    for (int i = 2; i < dir->nbinodes; i++)
        if ((dir->list)[i]->type == DIR)
            print_vfsinode((dir->list)[i], name);
}

static void print_vfsinode(s_vfsinode *inode, const char *path)
{
    switch (inode->type)
    {
        case DIR:
            print_vfsdir((s_vfsdir *)inode->node, path);
            break;
        case FILE:
            print_vfsfile((s_vfsfile *)inode->node);
            break;
        case EXEC:
            print_vfsexec((s_vfsexec *)inode->node);
            break;
        case DEV:
            print_vfsdev((s_vfsdev *)inode->node);
            break;
        default:
            break;
    }
}

void print_vfs(void)
{
    print_vfsinode(&vfsroot, "");
}
