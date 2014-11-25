#ifndef ATAGS_H
# define ATAGS_H

# include <stdint.h>
# include "graphics.h"

/* Usefull defines for atags parsing. */
# define ANONE   0x00000000
# define ACORE   0x54410001
# define AMEM    0x54410002
# define ATEXT   0x54410003
# define ARAMDSK 0x54410004
# define AINITRD 0x54420005
# define ASERIAL 0x54410006
# define AREV    0x54410007
# define AFB     0x54410008
# define ACMD    0x54410009

/* Atags structures */
typedef struct aheader
{
    uint32_t size;
    uint32_t tag;
} s_aheader;

typedef struct anone
{
    s_aheader header;
} s_anone;

typedef struct acore
{
    s_aheader header;
    uint32_t flags;
    uint32_t page_size;
    uint32_t root;
} s_acore;

typedef struct amem
{
    s_aheader header;
    uint32_t size;
    uint32_t addr;
} s_amem;

typedef struct aramdsk
{
    s_aheader header;
    uint32_t flags;
    uint32_t size;
    uint32_t start;
} s_aramdsk;

typedef struct ainitrd
{
    s_aheader header;
    uint32_t addr;
    uint32_t size;
} s_ainitrd;

typedef struct aserial
{
    s_aheader header;
    uint32_t low;
    uint32_t high;
} s_aserial;

typedef struct arev
{
    s_aheader header;
    uint32_t rev;
} s_arev;

typedef struct afb
{
    s_aheader header;
    uint16_t width;
    uint16_t height;
    uint16_t depth;
    uint16_t linelength;
    uint32_t address;
    uint32_t size;
    uint8_t redsize;
    uint8_t redpos;
    uint8_t greensize;
    uint8_t greenpos;
    uint8_t bluesize;
    uint8_t bluepos;
    uint8_t reservedsize;
    uint8_t reservedpos;
} s_afb;

typedef struct acmd
{
    s_aheader header;
    char cmd;
} s_acmd;

/* External atags functions. */
void print_atags(s_aheader *atags);
uint32_t get_root(void);

#endif /* !ATAGS_H */
