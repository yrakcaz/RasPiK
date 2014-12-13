#include "fs/vffs.h"

int create_vfile(s_vffs *vffs, const char *name)
{
    if (vffs->nbfile >= NBMAX_FILE)
        return -1;

    for (int i = 0; i < vffs->nbfile; i++)
        if (!strcmp(vffs->list[i]->name, name))
            return -1;

    s_vfile *file = kmalloc(sizeof (s_vfile));
    if (!file)
        return -1;

    file->name = name;
    file->size = 0;
    file->perm = PERM_READ | PERM_WRITE;
    file->lock = 0;

    file->data = kmalloc(sizeof (char));
    if (!file->data)
    {
        kfree(file);
        return -1;
    }

    file->data[0] = EOF;
    vffs->list[vffs->nbfile++] = file;

    return 0;
}

s_vfile *getnode_vffs(s_vffs *vffs, const char *name)
{
    for (int i = 0; i < vffs->nbfile; i++)
        if (!strcmp(vffs->list[i]->name, name))
            return vffs->list[i];
    return NULL;
}

int remove_vfile(s_vffs *vffs, const char *name)
{
    for (int i = 0; i < vffs->nbfile; i++)
    {
        if (!strcmp(vffs->list[i]->name, name))
        {
            kfree(vffs->list[i]->data);
            kfree(vffs->list[i]);
            for (int j = i; j < vffs->nbfile - 1; j++)
                vffs->list[j] = vffs->list[j + 1];
            vffs->nbfile--;
            return 0;
        }
    }
    return -1;
}

int read_vfile(s_vfile *file, uint32_t offset, void *buff, uint32_t len)
{
    if ((file->perm & PERM_READ) != PERM_READ)
        return -1;
    int count = 0;
    for (int i = offset; i < file->size && file->data[i] != EOF && count < len; i++)
    {
        ((char *)buff)[count] = file->data[i];
        count++;
    }
    return count;
}

int write_vfile(s_vfile *file, uint32_t offset, const void *buff, uint32_t len)
{
    if (((file->perm & PERM_WRITE) != PERM_WRITE) || file->lock == 1)
        return -1;
    int count = 0;
    file->lock = 1;

    if (offset + len + 1 > file->size)
        file->data = krealloc(file->data, offset + len + 1);

    int i;
    for (i = offset; i < len; i++)
    {
        file->data[i] = ((char *)buff)[count];
        count++;
    }
    file->data[i] = EOF;

    file->lock = 0;
    return count;
}

int chmod_vfile(s_vffs *vffs, const char *name, int perm)
{
    for (int i = 0; i < vffs->nbfile; i++)
    {
        if (!strcmp(vffs->list[i]->name, name))
        {
            vffs->list[i]->perm = perm;
            return 0;
        }
    }
    return -1;
}

const char **readdir_vffs(s_vffs *vffs)
{
    const char **ret = kmalloc ((vffs->nbfile + 1) * sizeof (char *));
    if (!ret)
        return NULL;

    int i;
    for (i = 0; i < vffs->nbfile; i++)
        ret[i] = vffs->list[i]->name;
    ret[i] = NULL;

    return ret;
}

s_vffs *create_vffs(void)
{
    s_vffs *vffs = kmalloc(sizeof (s_vffs));
    if (!vffs)
        return NULL;

    vffs->nbfile = 0;
    for (int i = 0; i < NBMAX_FILE; i++)
        vffs->list[i] = NULL;

    return vffs;
}

void remove_vffs(s_vffs *vffs)
{
    for (int i = 0; i < NBMAX_FILE; i++)
    {
        if (vffs->list[i])
        {
            kfree(vffs->list[i]->data);
            kfree(vffs->list[i]);
        }
    }
    kfree(vffs);
}
