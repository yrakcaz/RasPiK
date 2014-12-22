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
        if (parts[i].type != TYPEFAT32)
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

const char **readdir_fat32(s_fat32 *fat32)
{
    s_fatdir *dir = kmalloc(fat32->cluslen * fat32->seclen);
    if (!dir)
        return NULL;

    int fd = open(fat32->devpath, O_RDONLY);
    if (fd < 0)
    {
        kfree(dir);
        return NULL;
    }

    if (seek(fd, fat32->root, SEEK_SET) < 0)
    {
        close(fd);
        kfree(dir);
        return NULL;
    }

    uint32_t size = fat32->cluslen * fat32->seclen / 512;
    if (read(fd, dir, size) != size)
    {
        close(fd);
        kfree(dir);
        return NULL;
    }

    uint32_t maxnbfile = fat32->cluslen * fat32->seclen / sizeof (s_fatdir);
    const char **ret = kmalloc(maxnbfile * sizeof (char *));
    if (!ret)
    {
        close(fd);
        kfree(dir);
        return NULL;
    }

    uint32_t count = 0;
    for (uint32_t i = 0; i < maxnbfile; i++)
    {
        if ((dir[i].attrs & FAT32_VOL) || (dir[i].attrs & FAT32_DIR))
            continue;
        if (dir[i].name[0] == 0xE5)
            continue;
        if (dir[i].name[0] == 0x00)
            break;

        char *filename = kmalloc(13);
        int pos = 0;
        for (int j = 0; j < 8; j++)
            filename[pos++] = dir[i].name[j];
        filename[pos++] = '.';
        for (int j = 0; j < 3; j++)
            filename[pos++] = dir[i].ext[j];

        count++;
    }

    ret[count] = NULL;

    close(fd);
    kfree(dir);
    return ret;
}
