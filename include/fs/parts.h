#ifndef PARTS_H
# define PARTS_H

# include "mem.h"

typedef struct mbrentry
{
    uint8_t status;
    uint8_t start[3];
    uint8_t type;
    uint8_t end[3];
    uint32_t first;
    uint32_t sectors;
}__attribute__((packed)) s_mbrentry;

typedef struct mbr
{
    uint8_t code[446];
    s_mbrentry parts[4];
    uint8_t sig[2];
}__attribute__((packed)) s_mbr;

typedef struct part
{
    uint32_t start;
    uint32_t size;
    uint8_t type;
} s_part;

s_part *get_parts(s_mbr *mbr);
void print_parts(s_mbr *mbr); //JUST FOR DEBUGING...

#endif /* !PARTS_H */
