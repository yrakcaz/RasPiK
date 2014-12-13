#include "process.h"
#include "fs/vffs.h"
#include "fs/vfs.h"

static char **parse_path(const char *path)
{
    char **ret = kmalloc (3 * sizeof (char *));
    ret[0] = kmalloc(strlen(path) * sizeof (char));
    ret[1] = kmalloc(strlen(path) * sizeof (char));
    ret[2] = NULL;

    int i = path[0] == '/' ? 1 : 0;
    int j = 0;
    while (path[i] && path[i] != '/')
    {
        ret[0][j] = path[i];
        j++;
        i++;
    }
    ret[0][j] = '\0';

    if (!path[i])
    {
        kfree(ret[0]);
        kfree(ret[1]);
        kfree(ret);
        return NULL;
    }

    i++;
    j = 0;
    while (path[i])
    {
        ret[1][j] = path[i];
        i++;
        j++;
    }
    ret[1][j] = '\0';

    return ret;
}

static void *getnode(const char *path, int *type, int mode, int *offset)
{
    char **parsed = parse_path(path);
    if (!parsed)
        return NULL;

    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        if (!strcmp(parsed[0], vfsroot.list[i].name))
        {
            *type = vfsroot.list[i].type;
            if (*type == VFILES)
            {
                s_vfile *file = getnode_vffs(vfsroot.list[i].fs, parsed[1]);
                if (!file)
                {
                    if ((mode & O_CREAT) == O_CREAT)
                    {
                        if (create_vfile(vfsroot.list[i].fs, parsed[1]) < 0)
                            break;
                        file = getnode_vffs(vfsroot.list[i].fs, parsed[i]);
                        if (!file)
                            break;
                    }
                    else
                        break;
                }
                if ((mode & O_RDWR) == O_RDWR)
                {
                    if ((file->perm & PERM_WRITE) != PERM_WRITE)
                        break;
                    if ((mode & O_APPEND) == O_APPEND)
                        *offset = file->size;
                    else
                    {
                        *offset = 0;
                        file->data = krealloc(file->data, sizeof (char));
                        file->data[0] = EOF;
                        file->size = 0;
                    }
                }
                return file;
            }
        }
    }

    kfree(parsed[0]);
    kfree(parsed[1]);
    kfree(parsed);
    return NULL;
}

int open(const char *path, int mode)
{
    int type, offset;
    void *node = getnode(path, &type, mode, &offset);
    if (!node)
        return -1;

    for (int i = 0; i < NBMAX_FD; i++)
    {
        if (current_process->fd_table[i].addr == NULL)
        {
            current_process->fd_table[i].type = type;
            current_process->fd_table[i].addr = node;
            current_process->fd_table[i].offset = offset;
            return i;
        }
    }

    return -1;
}

int close(int fd)
{
    if (fd >= NBMAX_FD || fd < 0)
        return -1;
    if (current_process->fd_table[fd].addr)
    {
        current_process->fd_table[fd].addr = NULL;
        return fd;
    }
    else
        return -1;
}

const char **readdir(const char *path)
{
    if (!strcmp(path, "") || !strcmp(path, "/"))
    {
        const char **dirlist = kmalloc((vfsroot.nbmpoints + 1) * sizeof (char *));
        int i;
        for (i = 0; i < vfsroot.nbmpoints; i++)
            dirlist[i] = vfsroot.list[i].name;
        dirlist[i] = NULL;
        return dirlist;
    }
    else
    {
        int offset = path[0] == '/' ? 1 : 0;
        for (int i = 0; i < vfsroot.nbmpoints; i++)
            if (!strcmp(path + offset, vfsroot.list[i].name))
                if (vfsroot.list[i].type == VFILES)
                    return readdir_vffs(vfsroot.list[i].fs);
        return NULL;
    }
}

int init_vfs(void)
{
    vfsroot.nbmpoints = 0;
    return 1;
}
