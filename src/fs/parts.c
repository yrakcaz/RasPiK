#include "fs/parts.h"

s_part *get_parts(s_mbr *mbr)
{
    if (mbr->sig != 0xAA55)
        return NULL;

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
        klog("PART[");
        klog(itoa(i, 10));
        klog("]\n");
        klog("\tStart: ");
        klog(itoa(parts[i].start, 10));
        klog("\n");
        klog("\tSize: ");
        klog(itoa(parts[i].size, 10));
        klog("\n");
        klog("\tType: ");
        klog(itoa(parts[i].type, 10));
        klog("\n");
    }
    kfree(parts);
}
