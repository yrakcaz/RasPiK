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
