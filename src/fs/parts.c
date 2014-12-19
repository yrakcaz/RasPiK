#include "fs/parts.h"

s_part *get_parts(s_mbr *mbr)
{
    if ((mbr->sig != 0xAA55) && (mbr->sig != 0x55AA))
    {
        klog("Not an mbr!\n", 12, RED);
        return NULL;
    }

    s_part *parts = kmalloc(4 * sizeof (s_part));

    int i;
    for (i = 0; i < 4; i++)
    {
        s_mbrentry *part = &mbr->parts[i];
        if (part->first)
        {
            parts[i].start = part->first;
            parts[i].size = part->sectors;
            parts[i].type = part->type;
        }
        else
            parts[i].size = 0;
    }

    return parts;
}

void print_parts(s_mbr *mbr)
{
    s_part *parts = get_parts(mbr);
    for (int i = 0; i < 4; i++)
    {
        klog("PART[", 5, BLUE);
        klog(itoa(i, 10), strlen(itoa(i, 10)), BLUE);
        klog("] :\n\t", 5, BLUE);
        klog("Start: ", 7, BLUE);
        klog(itoa(parts[i].start, 10), strlen(itoa(parts[i].start, 10)), BLUE);
        klog("\n\t", 2, BLUE);
        klog("Size: ", 6, BLUE);
        klog(itoa(parts[i].size, 10), strlen(itoa(parts[i].size, 10)), BLUE);
        klog("\n\t", 2, BLUE);
        klog("Type: ", 6, BLUE);
        klog(itoa(parts[i].type, 10), strlen(itoa(parts[i].type, 10)), BLUE);
        klog("\n\n", 2, BLUE);
    }
    kfree(parts);
}
