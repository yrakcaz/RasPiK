#include "fs/fat32.h"
#include "fs/vfs.h"

s_fat32 *create_fat32(const char *devpath)
{
    int fd = open(devpath, O_RDWR);
    if (fd < 0)
        return NULL;

    s_mbr *mbr = kmalloc(sizeof (s_mbr));
    if (!mbr)
        return NULL;

    if (read(fd, mbr, sizeof (s_mbr)) < 0)
        return NULL;

    s_part *parts = get_parts(mbr);
    kfree(mbr);
    if (!parts)
        return NULL;

    s_fat32 *fat32 = NULL;
    for (int i = 0; i < 4; i++)
    {
        if (!(parts[i].size))
            continue;
        if (parts[i].type != TYPEFAT32 && parts[i].type != TYPEFAT32_LBA)
            continue;

        fat32 = kmalloc(sizeof (s_fat32));
        if (!fat32)
            break;
        s_bpb *bpb = kmalloc(BLK_SIZE);
        if (!bpb)
        {
            kfree(fat32);
            break;
        }

        seek(fd, parts[i].start, SEEK_SET);
        if (read(fd, bpb, BLK_SIZE) < 0)
        {
            kfree(fat32);
            break;
        }

        fat32->devpath = devpath;
        fat32->seclen = bpb->bpersec;
        fat32->cluslen = bpb->nosecperc;
        fat32->nbfats = bpb->nofat;
        fat32->fatbits = 32;
        fat32->fatlen = bpb->ebpb.secperfat;
        fat32->firstfat = parts[i].start + bpb->noressec;
        fat32->root = fat32->firstfat + fat32->nbfats * fat32->fatlen;

        kfree(bpb);
        break;
    }

    kfree(parts);
    close(fd);
    return fat32;
}

void remove_fat32(s_fat32 *fat32)
{
    kfree(fat32);
}

static int byte_to_int(char *buf)
{
    return (buf [0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24));
}

static int get_entry(s_fat32 *info, uint32_t cluster)
{
    cluster *= sizeof (int);
    uint32_t index = (cluster % BLK_SIZE);
    uint32_t begin = info->firstfat;
    uint32_t sectoread = begin + (cluster / BLK_SIZE);

    int fd = open(info->devpath, O_RDONLY);
    if (fd < 0)
        return -1;
    if (seek(fd, sectoread, SEEK_SET) < 0)
    {
        close(fd);
        return -1;
    }

    char *buff = kmalloc(BLK_SIZE);
    if (!buff)
    {
        close(fd);
        return -1;
    }

    if (read(fd, buff, BLK_SIZE) != BLK_SIZE)
    {
        close(fd);
        kfree(buff);
        return -1;
    }

    int entry = byte_to_int(&buff[index]);

    close(fd);
    kfree(buff);
    return entry;
}

const char **readdir_fat32(s_fat32 *fat32)
{
    uint32_t cluster_size = fat32->cluslen * fat32->seclen;
    uint32_t entries_per_cluster = cluster_size / sizeof (s_fatdir);
    uint32_t max_entries = entries_per_cluster * 8;

    const char **ret = kmalloc((max_entries + 1) * sizeof (char *));
    if (!ret)
        return NULL;

    uint32_t count = 0;
    uint32_t sector = fat32->root;
    uint32_t curclus = 2;
    int done = 0;

    while (!done)
    {
        s_fatdir *dir = kmalloc(cluster_size);
        if (!dir)
            break;

        int fd = open(fat32->devpath, O_RDONLY);
        if (fd < 0)
        {
            kfree(dir);
            break;
        }

        if (seek(fd, sector, SEEK_SET) < 0 || read(fd, dir, cluster_size) != cluster_size)
        {
            close(fd);
            kfree(dir);
            break;
        }
        close(fd);

        for (uint32_t i = 0; i < entries_per_cluster && count < max_entries; i++)
        {
            if ((dir[i].attrs & FAT32_VOL) || (dir[i].attrs & FAT32_DIR))
                continue;
            if (dir[i].name[0] == FAT32_DELETED)
                continue;
            if (dir[i].name[0] == FAT32_DIRENT_END) { done = 1; break; }

            char *filename = kmalloc(13 * sizeof (char));
            if (!filename) { done = 1; break; }

            int pos = 0;
            for (int j = 0; j < 8 && dir[i].name[j] && dir[i].name[j] != ' '; j++)
                filename[pos++] = dir[i].name[j];
            filename[pos++] = '.';
            for (int j = 0; j < 3; j++)
                filename[pos++] = dir[i].ext[j];
            filename[pos] = '\0';

            ret[count++] = filename;
        }

        kfree(dir);

        if (done)
            break;

        int entry = get_entry(fat32, curclus);
        if (entry < 0)
            break;
        entry &= FAT32_CLUSTER_MASK;
        if (entry >= FAT32_CLUSTER_BAD)
            break;

        curclus = entry;
        sector = fat32->root + ((curclus - 2) * fat32->cluslen);
    }

    ret[count] = NULL;
    return ret;
}

static char upper(char c)
{
    if (c >= 'a' && c <= 'z')
        return (c - 'a' + 'A');
    return c;
}

static int check_filename(s_fatdir file, const char *name)
{
    for (int i = 0; i < 8 && file.name[i] && file.name[i] != ' '; i++)
        if (file.name[i] != upper(name[i]))
            return 0;
    for (int i = strlen(name) - 1, j = 2; j >= 0; j--, i--)
        if (file.ext[j] != upper(name[i]))
            return 0;
    return 1;
}

s_fatfile *get_node_fat32(s_fat32 *fat32, const char *name)
{
    uint32_t cluster_size = fat32->cluslen * fat32->seclen;
    uint32_t entries_per_cluster = cluster_size / sizeof (s_fatdir);

    uint32_t sector = fat32->root;
    uint32_t curclus = 2;

    while (1)
    {
        s_fatdir *dir = kmalloc(cluster_size);
        if (!dir)
            return NULL;

        int fd = open(fat32->devpath, O_RDONLY);
        if (fd < 0)
        {
            kfree(dir);
            return NULL;
        }

        if (seek(fd, sector, SEEK_SET) < 0 || read(fd, dir, cluster_size) != cluster_size)
        {
            close(fd);
            kfree(dir);
            return NULL;
        }
        close(fd);

        for (uint32_t i = 0; i < entries_per_cluster; i++)
        {
            if ((dir[i].attrs & FAT32_VOL) || (dir[i].attrs & FAT32_DIR))
                continue;
            if (dir[i].name[0] == FAT32_DELETED)
                continue;
            if (dir[i].name[0] == FAT32_DIRENT_END)
            {
                kfree(dir);
                return NULL;
            }

            if (check_filename(dir[i], name))
            {
                s_fatfile *file = kmalloc(sizeof (s_fatfile));
                if (!file)
                {
                    kfree(dir);
                    return NULL;
                }
                file->info = fat32;
                file->dir = dir + i;
                return file;
            }
        }

        kfree(dir);

        int entry = get_entry(fat32, curclus);
        if (entry < 0)
            return NULL;
        entry &= FAT32_CLUSTER_MASK;
        if (entry >= FAT32_CLUSTER_BAD)
            return NULL;

        curclus = entry;
        sector = fat32->root + ((curclus - 2) * fat32->cluslen);
    }
}

static int read_shortfile(uint32_t nbsectors, s_fatfile *file, uint32_t sector,
                          uint32_t len, uint32_t *total, void *buf)
{
    for (uint32_t i = 0; i < nbsectors; i++)
    {
        char *buff = kmalloc(BLK_SIZE);
        if (!buff)
            return -1;
        int fd = open(file->info->devpath, O_RDONLY);
        if (fd < 0 || seek(fd, sector, SEEK_SET) < 0)
        {
            if (fd < 0)
                close(fd);
            kfree(buff);
            return -1;
        }

        if (read(fd, buff, BLK_SIZE) <= 0)
        {
            close(fd);
            kfree(buff);
            return -1;
        }
        uint32_t nbcpy = len > BLK_SIZE ? BLK_SIZE : len;
        for (int j = 0; j < nbcpy; j++)
            ((char *)buf)[(*total)++] = buff[j];
        len -= nbcpy;
        sector++;

        close(fd);
        kfree(buff);
    }
    return 0;
}

static int read_longfile(uint32_t nbsectors, s_fatfile *file, uint32_t sector,
                         uint32_t len, uint32_t *total, void *buf)
{
    uint32_t nbclusters = nbsectors / file->info->cluslen;
    if (nbsectors % file->info->cluslen)
        nbclusters++;
    for (uint32_t i = 0; i < nbclusters; i++)
    {
        for (uint32_t j = 0; j < file->info->cluslen; j++)
        {
            int fd = open(file->info->devpath, O_RDONLY);
            if (fd < 0)
                return -1;
            if (seek(fd, sector, SEEK_SET) < 0)
            {
                close(fd);
                return -1;
            }
            char *buff = kmalloc(BLK_SIZE);
            if (!buff)
            {
                close(fd);
                return -1;
            }
            if (read(fd, buff, BLK_SIZE) < 0)
            {
                kfree(buff);
                close(fd);
                return -1;
            }
            close(fd);

            uint32_t bread = len > 512 ? 512 : len;
            for (int k = 0; k < bread; k++)
                ((char *)buf + *total)[k] = buff[k];
            kfree(buff);

            len -= bread;
            *total += bread;
            if (len <= 0)
                break;
            sector++;
        }
        if (len <= 0)
            break;

        sector--;
        uint32_t curclus = ((sector - file->info->root) / file->info->cluslen) + 2;
        int entry = get_entry(file->info, curclus);
        if (entry < 0)
            break;

        entry &= FAT32_CLUSTER_MASK;
        if (entry >= FAT32_CLUSTER_BAD)
            break;

        sector = file->info->root + ((entry - 2) * file->info->cluslen);
    }

    return 0;
}

int read_fat32file(s_fatfile *file, uint32_t *offset, void *buf, uint32_t len)
{
    if (len <= 0)
        return len;

    uint32_t left = file->dir->size - *offset;
    if (left < len)
        len = left;

    uint32_t firstclus = (file->dir->clhigh << 16) | file->dir->cllow;
    uint32_t sector = file->info->root + ((firstclus - 2) * file->info->cluslen);
    uint32_t nbsectors = len / file->info->seclen;
    if (nbsectors % BLK_SIZE || !nbsectors)
        nbsectors++;

    uint32_t total = 0;

    char *buff = kmalloc(file->dir->size + 1);
    if (nbsectors <= file->info->cluslen)
    {
        if (read_shortfile(nbsectors, file, sector, file->dir->size, &total, buff) < 0)
        {
            kfree(buff);
            return -1;
        }
    }
    else
    {
        if (read_longfile(nbsectors, file, sector, file->dir->size, &total, buff) < 0)
        {
            kfree(buff);
            return -1;
        }
    }

    int i;
    for (i = 0; i < len && (*offset + i) < file->dir->size; i++)
        ((char *)buf)[i] = buff[*offset + i];
    ((char *)buf)[i] = 0;

    *offset += i;
    return i;
}
