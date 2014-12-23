#ifndef FAT32_H
# define FAT32_H

# include "fs/parts.h"

/*
** For the moment, LFN are not handled,
** and spaces are not allowed in file
** names...
*/

# define TYPEFAT32 0x0B

# define BLK_SIZE  512

# define FAT32_RO  (1 << 0)
# define FAT32_HID (1 << 1)
# define FAT32_SYS (1 << 2)
# define FAT32_VOL (1 << 3)
# define FAT32_DIR (1 << 4)
# define FAT32_ACH (1 << 5)

typedef struct ebpb32
{
    uint32_t secperfat;
    uint16_t flags;
    uint16_t fatversion;
    uint32_t rootcluster;
    uint16_t fsinfosec;
    uint16_t backupsec;
    uint8_t reserved[12];
    uint8_t drvnum;
    uint8_t ntflags;
    uint8_t sig;
    uint32_t serial;
    uint8_t label[11];
    uint8_t sysid[8];
    uint8_t bootcode[420];
    uint16_t partsig;
}__attribute__((packed)) s_ebpb32;

typedef struct bpb
{
    uint8_t bootjp[3];
    uint8_t oemname[8];
    uint16_t bpersec;
    uint8_t nosecperc;
    uint16_t noressec;
    uint8_t nofat;
    uint16_t nodir;
    uint16_t totalsec;
    uint8_t desctype;
    uint16_t nosecperfat;
    uint16_t nosecpertrack;
    uint16_t noheads;
    uint32_t nohidd;
    uint32_t laamosec;
    s_ebpb32   ebpb;
}__attribute__((packed)) s_bpb;

typedef struct fatdir
{
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attrs;
    uint8_t res;
    uint8_t crms;
    uint16_t crtime;
    uint16_t crdate;
    uint16_t acdate;
    uint16_t clhigh;
    uint16_t modtime;
    uint16_t moddate;
    uint16_t cllow;
    uint32_t size;
} s_fatdir;

typedef struct lfn
{
    uint8_t ord;
    uint16_t chr1[5];
    uint8_t attrs;
    uint8_t res1;
    uint8_t cksum;
    uint16_t chr2[6];
    uint16_t res2;
    uint16_t chr3[2];
}__attribute__((packed)) s_lfn;

typedef struct fat32
{
    const char *devpath;
    uint16_t seclen;
    uint8_t cluslen;
    uint8_t nbfats;
    uint8_t fatbits;
    uint32_t fatlen;
    uint32_t firstfat;
    uint32_t root;
} s_fat32;

s_fat32 *create_fat32(const char *devpath);
void remove_fat32(s_fat32 *fat32);
const char **readdir_fat32(s_fat32 *fat32);
s_fatdir *getnode_fat32(s_fat32 *fat32, const char *name);

//TODO:
int create_fat32file(s_fat32 *fat32, const char *name);
int remove_fat32file(s_fat32 *fat32, const char *name);
int read_fat32file(s_fat32 *fat32, uint32_t *offset, void *buf, uint32_t len);
int write_fat32file(s_fat32 *fat32, uint32_t *offset, const void *buf, uint32_t len);
int chmod_fat32file(s_fat32 *fat32, const char *name, int perm);

#endif /* !FAT32_H */
