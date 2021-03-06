#include "process.h"
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
                        file = getnode_vffs(vfsroot.list[i].fs, parsed[1]);
                        if (!file)
                            break;
                    }
                    else
                        break;
                }
                if ((mode & O_RDWR) == O_RDWR)
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
                return file;
            }
            if (*type == FAT32)
            {
                s_fatfile *file = getnode_fat32(vfsroot.list[i].fs, parsed[1]);
                if (!file)
                {
                    //TODO : Create file if necessary..
                    break;
                }
                if ((mode & O_RDWR) == O_RDWR)
                {
                    //TODO : Fix it!
                    break;
                }
                if ((mode & O_APPEND) == O_APPEND)
                    *offset = file->dir->size;
                else
                    *offset = 0;
                return file;
            }
            if (*type == DEVICES)
            {
                s_device *device = getnode_devfs(vfsroot.list[i].fs, parsed[1]);
                if (!device)
                    break;
                if ((mode & O_RDWR) == O_RDWR)
                    if ((device->perm & PERM_WRITE) != PERM_WRITE)
                        break;
                *offset = 0;
                return device;
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
            current_process->fd_table[i].mode = mode;
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

int read(int fd, void *buf, uint32_t len)
{
    if (fd > NBMAX_FD || current_process->fd_table[fd].addr == NULL)
        return -1;

    void *addr = current_process->fd_table[fd].addr;
    switch(current_process->fd_table[fd].type)
    {
        case VFILES:
            return read_vfile(addr, &(current_process->fd_table[fd].offset), buf, len);
        case DEVICES:
            return ((s_device *)addr)->driver->read(addr, &(current_process->fd_table[fd].offset), buf, len);
        case FAT32:
            return read_fat32file(addr, &(current_process->fd_table[fd].offset), buf, len);
        default:
            return -1;
    }
}

int write(int fd, const void *buf, uint32_t len)
{
    if (fd > NBMAX_FD || current_process->fd_table[fd].addr == NULL ||
        (current_process->fd_table[fd].mode & O_RDONLY) == O_RDONLY)
        return -1;

    void *addr = current_process->fd_table[fd].addr;
    switch(current_process->fd_table[fd].type)
    {
        case VFILES:
            return write_vfile(addr, &(current_process->fd_table[fd].offset), buf, len);
        case DEVICES:
            return ((s_device *)addr)->driver->write(addr, &(current_process->fd_table[fd].offset), buf, len);
        default:
            return -1;
    }
}

int remove(const char *path)
{
    char **parsed = parse_path(path);
    if (!parsed)
        return -1;

    int ret = -1;

    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        if (!strcmp(vfsroot.list[i].name, parsed[0]))
        {
            switch (vfsroot.list[i].type)
            {
                case VFILES:
                    ret = remove_vfile(vfsroot.list[i].fs, parsed[1]);
                    break;
                case DEVICES:
                    ret = remove_device(vfsroot.list[i].fs, parsed[1]);
                    break;
                default:
                    break;
            }
            break;
        }
    }

    kfree(parsed[0]);
    kfree(parsed[1]);
    kfree(parsed);

    return ret;
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
        {
            if (!strcmp(path + offset, vfsroot.list[i].name))
            {
                if (vfsroot.list[i].type == VFILES)
                    return readdir_vffs(vfsroot.list[i].fs);
                if (vfsroot.list[i].type == DEVICES)
                    return readdir_devfs(vfsroot.list[i].fs);
                if (vfsroot.list[i].type == FAT32)
                    return readdir_fat32(vfsroot.list[i].fs);
            }
        }
        return NULL;
    }
}

int seek(int fd, uint32_t offset, int whence)
{
    if (fd > NBMAX_FD || current_process->fd_table[fd].addr == NULL)
        return -1;
    uint32_t len;
    switch (current_process->fd_table[fd].type)
    {
        case VFILES:
            len = ((s_vfile *)current_process->fd_table[fd].addr)->size;
            break;
        case DEVICES:
            len = 0;
            break;
        case FAT32:
            len = ((s_fatdir *)current_process->fd_table[fd].addr)->size;
            break;
        default:
            return -1;
    }
    switch (whence)
    {
        case SEEK_SET:
            current_process->fd_table[fd].offset = offset;
            break;
        case SEEK_CUR:
            current_process->fd_table[fd].offset += offset;
            break;
        case SEEK_END:
            current_process->fd_table[fd].offset = (len - 1) + offset;
            break;
        default:
            return -1;
    }
    return current_process->fd_table[fd].offset;
}

int ioctl(int fd, int cmd, int args)
{
    if (fd < 0 || fd > NBMAX_FD)
        return -1;

    void *addr = current_process->fd_table[fd].addr;
    if (!addr)
        return -1;

    switch(current_process->fd_table[fd].type)
    {
        case VFILES:
            return -1;
        case DEVICES:
            return ((s_device *)addr)->driver->ioctl(addr, cmd, args);
        case FAT32:
            return -1;
        default:
            return -1;
    }
}

int stat(int fd, s_stat *sb)
{
    if (fd < 0 || fd > NBMAX_FD || !sb)
        return -1;

    void *addr = current_process->fd_table[fd].addr;
    if (!addr)
        return -1;

    switch(current_process->fd_table[fd].type)
    {
        case VFILES:
            sb->st_size = ((s_vfile *)addr)->size;
            return -1;
        case DEVICES:
            return -1;
        case FAT32:
            sb->st_size = ((s_fatfile *)addr)->dir->size;
            return 0;
        default:
            return -1;
    }
}

static int add_vffs(const char *path)
{
    const char *name = path[0] == '/' ? path + 1 : path;
    int i;
    for (i = 0; i < NBMAX_MOUNTINGPOINT; i++)
        if (vfsroot.list[i].fs == NULL)
            break;
    if (i >= NBMAX_MOUNTINGPOINT)
        return -1;
    void *fs = create_vffs();
    if (!fs)
        return -1;
    vfsroot.list[i].name = name;
    vfsroot.list[i].type = VFILES;
    vfsroot.list[i].fs = fs;
    vfsroot.nbmpoints++;
    return i;
}

static int add_devfs(const char *path)
{
    const char *name = path[0] == '/' ? path + 1 : path;
    int i;
    for (i = 0; i < NBMAX_MOUNTINGPOINT; i++)
        if (vfsroot.list[i].fs == NULL)
            break;
    if (i >= NBMAX_MOUNTINGPOINT)
        return -1;
    void *fs = create_devfs();
    if (!fs)
        return -1;
    vfsroot.list[i].name = name;
    vfsroot.list[i].type = DEVICES;
    vfsroot.list[i].fs = fs;
    vfsroot.nbmpoints++;
    return i;
}

static int add_fat32(const char *devpath, const char *mountpath)
{
    const char *name = mountpath[0] == '/' ? mountpath + 1 : mountpath;
    int i;
    for (i = 0; i < NBMAX_MOUNTINGPOINT; i++)
        if (vfsroot.list[i].fs == NULL)
            break;
    if (i >= NBMAX_MOUNTINGPOINT)
        return -1;
    void *fs = create_fat32(devpath);
    if (!fs)
        return -1;
    vfsroot.list[i].name = name;
    vfsroot.list[i].type = FAT32;
    vfsroot.list[i].fs = fs;
    vfsroot.nbmpoints++;
    return i;
}

int mount(const char *devpath, const char *mountpath, int type)
{
    if (type == VFILES)
        return add_vffs(mountpath);
    if (type == DEVICES)
        return add_devfs(mountpath);
    if (type == FAT32)
        return add_fat32(devpath, mountpath);
    return -1;
}

int unmount(const char *path)
{
    if (path[0] == '/')
        path++;
    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        if (!strcmp(path, vfsroot.list[i].name))
        {
            if (vfsroot.list[i].type == VFILES)
                remove_vffs(vfsroot.list[i].fs);
            else if (vfsroot.list[i].type == DEVICES)
                remove_devfs(vfsroot.list[i].fs);
            else if (vfsroot.list[i].type == FAT32)
                remove_fat32(vfsroot.list[i].fs);
            else
                return -1;
            vfsroot.list[i].fs = NULL;
            vfsroot.nbmpoints--;
            for (int j = i; j < vfsroot.nbmpoints; j++)
                vfsroot.list[j] = vfsroot.list[j + 1];
            return 0;
        }
    }

    return -1;
}

int chmod(const char *path, int mode)
{
    char **parsed = parse_path(path);
    if (!parsed)
        return -1;

    int ret = -1;
    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        if (!strcmp(vfsroot.list[i].name, parsed[0]))
        {
            switch (vfsroot.list[i].type)
            {
                case VFILES:
                    ret = chmod_vfile(vfsroot.list[i].fs, parsed[1], mode);
                case DEVICES:
                    ret = chmod_device(vfsroot.list[i].fs, parsed[1], mode);
                default:
                    break;
            }
            break;
        }
    }

    kfree(parsed[0]);
    kfree(parsed[1]);
    kfree(parsed);

    return ret;
}

int insmod(const char *path, void *addr, s_driver *driver)
{
    char **parsed = parse_path(path);
    if (!path)
        return -1;

    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        if (!strcmp(parsed[0], vfsroot.list[i].name))
        {
            if (vfsroot.list[i].type != DEVICES)
                return -1;
            return add_device(vfsroot.list[i].fs, parsed[1], addr, driver);
        }
    }

    return -1;
}

void print_vfs(void)
{
    for (int i = 0; i < vfsroot.nbmpoints; i++)
    {
        switch (vfsroot.list[i].type)
        {
            case VFILES:
                klog("VFFS : ", 7, WHITE);
                klog(vfsroot.list[i].name, strlen(vfsroot.list[i].name), WHITE);
                klog("\n\t", 2, WHITE);
                break;
            case DEVICES:
                klog("DEVFS : ", 8, YELLOW);
                klog(vfsroot.list[i].name, strlen(vfsroot.list[i].name), YELLOW);
                klog("\n\t", 2, YELLOW);
                break;
            case FAT32:
                klog("FAT32 : ", 8, WHITE);
                klog(vfsroot.list[i].name, strlen(vfsroot.list[i].name), WHITE);
                klog("\n\t", 2, WHITE);
                break;
            default:
                break;
        }
        const char **dir = readdir(vfsroot.list[i].name);
        if (!dir)
            return;
        for (int j = 0; dir[j]; j++)
        {
            klog(dir[j], strlen(dir[j]), WHITE);
            klog(" ", 1, WHITE);
        }
        klog("\n\n", 2, WHITE);
    }
}

int init_vfs(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    vfsroot.nbmpoints = 0;
    if (mount("", "tmp", VFILES) < 0 || mount("", "dev", DEVICES) < 0)
    {
        klog("\b\b\b\bKO", 6, RED);
        klog("]\tVirtual File System initialization failed.\n", 45, WHITE);
        return 0;
    }

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tVirtual File System initialized!\n", 35, WHITE);
    return 1;
}

int mount_devices(void)
{
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    if (mount("/dev/sdcard", "sdcard", FAT32) < 0)
    {
        klog("\b\b\b\bKO", 6, RED);
        klog("]\tDevices mounting failed.\n", 27, WHITE);
        return 0;
    }

    wait(HUMAN_TIME / 2);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tDevices mounted!\n", 19, WHITE);
    return 1;
}
