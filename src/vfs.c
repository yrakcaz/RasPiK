#include "vfs.h"

static int inumber = 0;
static s_vfsinode vfsroot;

//static s_fd fd_table[NBMAX_FD];
//static s_dir dir_table[NBMAX_DIR];

static const char *pwd = "/";

int init_vfs(void)
{
    vfsroot.inumber = inumber++;
    vfsroot.type = DIR;
    vfsroot.name = "/";

    s_vfsdir *rootdir = kmalloc(sizeof (s_vfsdir));
    if (!rootdir)
        return 0;

    rootdir->name = "/";
    rootdir->nbinodes = 2;
    rootdir->list = kmalloc(rootdir->nbinodes * sizeof (s_vfsinode *));

    vfsroot.node = (void *)rootdir;

    s_vfsinode *dot = kmalloc(sizeof (s_vfsinode));
    if (!dot)
        return 0;
    s_vfsinode *doubledot = kmalloc(sizeof (s_vfsinode));
    if (!doubledot)
        return 0;

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

void chdir(const char *path)
{
    pwd = path;
}

const char *get_pwd(void)
{
    return pwd;
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
        if ((dir->list)[i] == DIR)
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
        default:
            break;
    }
}

void print_vfs(void)
{
    print_vfsinode(&vfsroot, "");
}
