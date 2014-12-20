#ifndef EMMC_H
# define EMMC_H

# include "mailbox.h"
# include "fs/vfs.h"

/*
** My RasPik project was made to be really simple, but an emmc driver could not be simple,
** so I took one that I found on the internet, then I adapted it to understand it,
** because I really needed one but I wouldn't spend too much time on it...
*/

# define EMMC_BASE      0x20300000

# define EMMC_CHAN      0x00000000

# define ON             1
# define OFF            0

# define SDCMD_REPNONE  0
# define SDCMD_REP136   (1 << 16)
# define SDCMD_REP48    (2 << 16)
# define SDCMD_REP48B   (3 << 16)
# define SDCMD_REPMSK   (3 << 16)
# define SDCMD_CRCEN    (1 << 19)

# define SD_REPNONE     SDCMD_REPNONE
# define SD_REPR1       (SDCMD_REP48 | SDCMD_CRCEN)
# define SD_REPR1B      (SDCMD_REP48B | SDCMD_CRCEN)
# define SD_REPR2       (SDCMD_REP136 | SDCMD_CRCEN)
# define SD_REPR3       SDCMD_REP48
# define SD_REPR4       SDCMD_REP136
# define SD_REPR5       SD_REPR1
# define SD_REPR5B      SD_REPR1B
# define SD_REPR6       SD_REPR5
# define SD_REPR7       SD_REPR6

# define APP_CMD        0x80000000
# define ACMD(X)        (X | APP_CMD)

# define SDCMD_DDHC     0
# define SDCMD_DMA      1
# define SDCMD_BLKCNT   (1 << 1)
# define SDCMD_DDCH     (1 << 4)
# define SDCMD_MULBLK   (1 << 5)
# define SDCMD_ISDATA   (1 << 21)
# define SDCMD_ABORT    (3 << 22)

# define SDCMD_INDEX(X) ((X) << 24)
# define SDCMD_RES(X)   0xFFFFFFFF
# define SD_READ        (SDCMD_ISDATA | SDCMD_DDCH)
# define SD_WRITE       (SDCMD_ISDATA | SDCMD_DDHC)

# define SDC_INT        (1 << 8)

# define SD_UNKNOWN     0
# define SD_V1          1
# define SD_V1_1        2
# define SD_V2          3
# define SD_V3          4
# define SD_V4          5

# define SDCLK_ID       400000
# define SDCLK_NORMAL   25000000
# define SDCLK_HIGH     50000000
# define SDCLK_100      100000000
# define SDCLK_208      208000000

# define TM_CMD_NONE    0
# define TM_CMD_12      1
# define TM_CMD_23      2
# define TM_CMD_RES     3

# define REP_TY_NONE    0
# define REP_TY_136     1
# define REP_TY_48      2
# define REP_TY_48BSY   3

# define CMD_TY_NORMAL  0
# define CMD_TY_SUSPEND 1
# define CMD_TY_RESUME  2
# define CMD_TY_ABORT   3

# define SDR12          0
# define SDR25          1
# define SDR50          2
# define SDR104         3
# define DDR50          4

# define PS200          0
# define PS400          1
# define PS400_2        2
# define PS600          3
# define PS700          4
# define PS900          5
# define PS900_2        6
# define PS1100         7

# define EMMC_IDLE      0
# define EMMC_SENDOP    1
# define EMMC_ALLCID    2
# define EMMC_RELADDR   3
# define EMMC_SETDSR    4
# define EMMC_IOSETOP   5
# define EMMC_SWFUNC    6
# define EMMC_SELCARD   7
# define EMMC_DESELCARD 7
# define EMMC_SELDECARD 7
# define EMMC_SENDIF    8
# define EMMC_SENDCSD   9
# define EMMC_SENDCID   10
# define EMMC_SWVOLT    11
# define EMMC_STOPTR    12
# define EMMC_SENDSTAT  13
# define EMMC_INACTST   15
# define EMMC_BLKLEN    16
# define EMMC_R1BLK     17
# define EMMC_RXBLK     18
# define EMMC_TUNBLK    19
# define EMMC_SPEEDCTRL 20
# define EMMC_BLKCNT    23
# define EMMC_W1BLK     24
# define EMMC_WXBLK     25
# define EMMC_PROGCSD   27
# define EMMC_SETWPROT  28
# define EMMC_CLRWPROT  29
# define EMMC_SENDWPROT 30
# define EMMC_ERWBLKST  32
# define EMMC_ERWBLKEND 33
# define EMMC_ERASE     38
# define EMMC_LOCK      42
# define EMMC_UNLOCK    42
# define EMMC_ACMD      55
# define EMMC_GENCMD    56

# define SDMA_BUFF      0x6000
# define SDMA_BUFFPA    (SDMA_BUFF + 0xC0000000)

typedef struct sdscr
{
    uint32_t scr[2];
    uint32_t bus_widths;
    int version;
} s_sdscr;

typedef struct sd
{
    s_sdscr scr;
    uint32_t rep0;
    uint32_t rep1;
    uint32_t rep2;
    uint32_t rep3;
    uint32_t blksz;
    uint32_t blks_tr;
    uint32_t cid[4];
    uint32_t *rcv_buf;
    uint32_t rca;
    uint32_t sup_sdhc;
    uint32_t ocr;
    uint32_t failed;
} s_sd;

typedef volatile union emmcstreg
{
    uint32_t raw;
    struct
    {
        uint32_t cmd_inhibit : 1;
        uint32_t dat_inhibit : 1;
        uint32_t dat_active  : 1;
        uint32_t reserved    : 5;
        uint32_t wtransfer   : 1;
        uint32_t rtransfer   : 1;
        uint32_t reserved2   : 10;
        uint32_t dat_lev0    : 4;
        uint32_t cmd_lev     : 1;
        uint32_t dat_lev1    : 4;
        uint32_t reserved3   : 3;
    } bits;
} u_emmcstreg;

typedef volatile union blkszcnt
{
    uint32_t raw;
    struct
    {
        uint32_t blksz    : 10;
        uint32_t reserved : 6;
        uint32_t blkcnt   : 16;
    } bits;
} u_blkszcnt;

typedef volatile union cmdtmreg
{
    uint32_t raw;
    struct
    {
        uint32_t reserved  : 1;
        uint32_t blkcnten  : 1;
        uint32_t autocmden : 2;
        uint32_t datadir   : 1;
        uint32_t mulblk    : 1;
        uint32_t reserved2 : 10;
        uint32_t repty     : 2;
        uint32_t crcen     : 1;
        uint32_t ixen      : 1;
        uint32_t isdata    : 1;
        uint32_t cmdty     : 2;
        uint32_t cmdidx    : 6;
        uint32_t reserved3 : 2;
    } bits;
} u_cmdtmreg;

typedef volatile union ctrl0reg
{
    uint32_t raw;
    struct
    {
        uint32_t hctl_dwidth : 1;
        uint32_t hctl_hsen   : 1;
        uint32_t reserved    : 2;
        uint32_t hctl_8bit   : 1;
        uint32_t reserved2   : 11;
        uint32_t gap_stop    : 1;
        uint32_t gap_restart : 1;
        uint32_t rwait_en    : 1;
        uint32_t gap_ien     : 1;
        uint32_t spi_mode    : 1;
        uint32_t boot_en     : 1;
        uint32_t alt_booten  : 1;
        uint32_t reserved3   : 9;
    } bits;
} u_ctrl0reg;

typedef volatile union ctrl1reg
{
    uint32_t raw;
    struct
    {
        uint32_t clk_intlen  : 1;
        uint32_t clk_stable  : 1;
        uint32_t clk_en      : 1;
        uint32_t reserved    : 2;
        uint32_t clk_gensel  : 1;
        uint32_t clk_freqms2 : 2;
        uint32_t clk_freq8   : 8;
        uint32_t data_taunit : 4;
        uint32_t reserved2   : 4;
        uint32_t srst_hc     : 1;
        uint32_t srst_cmd    : 1;
        uint32_t srst_data   : 1;
        uint32_t reserved3   : 5;
    } bits;
} u_ctrl1reg;

typedef volatile union intreg
{
    uint32_t raw;
    struct
    {
        uint32_t cmd_done  : 1;
        uint32_t data_done : 1;
        uint32_t blk_gap   : 1;
        uint32_t reserved  : 1;
        uint32_t wready    : 1;
        uint32_t rready    : 1;
        uint32_t reserved2 : 2;
        uint32_t card      : 1;
        uint32_t reserved3 : 3;
        uint32_t retune    : 1;
        uint32_t bootack   : 1;
        uint32_t endboot   : 1;
        uint32_t error     : 1;
        uint32_t cto_err   : 1;
        uint32_t ccrc_err  : 1;
        uint32_t cend_err  : 1;
        uint32_t cbad_err  : 1;
        uint32_t dto_err   : 1;
        uint32_t dcrc_err  : 1;
        uint32_t dend_err  : 1;
        uint32_t reserved4 : 1;
        uint32_t acmd_err  : 1;
        uint32_t reserved5 : 7;
    } bits;
} u_intreg;

typedef volatile union intmskreg
{
    uint32_t raw;
    struct
    {
        uint32_t cmd_done  : 1;
        uint32_t data_done : 1;
        uint32_t blk_gap   : 1;
        uint32_t reserved  : 1;
        uint32_t wready    : 1;
        uint32_t rready    : 1;
        uint32_t reserved2 : 2;
        uint32_t card      : 1;
        uint32_t reserved3 : 3;
        uint32_t retune    : 1;
        uint32_t bootack   : 1;
        uint32_t endboot   : 1;
        uint32_t reserved4 : 1;
        uint32_t cto_err   : 1;
        uint32_t ccrc_err  : 1;
        uint32_t cend_err  : 1;
        uint32_t cbad_err  : 1;
        uint32_t dto_err   : 1;
        uint32_t dcrc_err  : 1;
        uint32_t dend_err  : 1;
        uint32_t reserved5 : 1;
        uint32_t acmd_err  : 1;
        uint32_t reserved6 : 7;
    } bits;
} u_intmskreg;

typedef volatile union ctrl2reg
{
    uint32_t raw;
    struct
    {
        uint32_t acnox_err  : 1;
        uint32_t acto_err   : 1;
        uint32_t accrc_err  : 1;
        uint32_t acend_err  : 1;
        uint32_t acbad_err  : 1;
        uint32_t reserved   : 2;
        uint32_t notc12_err : 1;
        uint32_t reserved2  : 8;
        uint32_t uhsmode    : 3;
        uint32_t reserved3  : 3;
        uint32_t tunon      : 1;
        uint32_t tuned      : 1;
        uint32_t reserved4  : 8;
    } bits;
} u_ctrl2reg;

typedef volatile union fifocfgreg
{
    uint32_t raw;
    struct
    {
        uint32_t rd_thrsh : 3;
        uint32_t reserved : 29;
    } bits;
} u_fifocfgreg;

typedef volatile union fifoenreg
{
    uint32_t raw;
    struct
    {
        uint32_t enable   : 1;
        uint32_t reserved : 31;
    } bits;
} u_fifoenreg;

typedef volatile union tunestepreg
{
    uint32_t tune_step : 3;
    uint32_t reserved  : 29;
} u_tunestepreg;

typedef volatile union tunestepstdreg
{
    uint32_t raw;
    struct
    {
        uint32_t steps    : 6;
        uint32_t reserved : 26;
    } bits;
} u_tunestepstdreg;

typedef volatile union tunestepddrreg
{
    uint32_t raw;
    struct
    {
        uint32_t steps    : 6;
        uint32_t reserved : 26;
    } bits;
} u_tunestepddrreg;

typedef volatile union spiintreg
{
    uint32_t raw;
    struct
    {
        uint32_t select   : 8;
        uint32_t reserved : 24;
    } bits;
} u_spiintreg;

typedef volatile union slotisrreg
{
    uint32_t raw;
    struct
    {
        uint32_t slotst    : 8;
        uint32_t reserved  : 8;
        uint32_t sdversion : 8;
        uint32_t vendor    : 8;
    } bits;
} u_slotisrreg;

typedef volatile union dbgselreg
{
    uint32_t raw;
    struct
    {
        uint32_t select   : 1;
        uint32_t reserved : 31;
    } bits;
} u_dbgselreg;

typedef volatile struct emmc
{
    uint32_t arg2;
    u_blkszcnt blkszcnt;
    uint32_t arg1;
    u_cmdtmreg cmdtm;
    uint32_t rep0;
    uint32_t rep1;
    uint32_t rep2;
    uint32_t rep3;
    uint32_t data;
    u_emmcstreg status;
    u_ctrl0reg ctrl0;
    u_ctrl1reg ctrl1;
    u_intreg interrupt;
    u_intmskreg intmsk;
    u_intreg inten;
    u_ctrl2reg ctrl2;
    uint32_t cap0;
    uint32_t cap1;
    uint32_t reserved;
    uint32_t reserved1;
    u_intreg intforce;
    uint32_t reserved2;
    uint32_t reserved3;
    uint32_t reserved4;
    uint32_t reserved5;
    uint32_t reserved6;
    uint32_t reserved7;
    uint32_t reserved8;
    uint32_t timeout;
    u_dbgselreg dbgsel;
    uint32_t reserved9;
    uint32_t reserved10;
    u_fifocfgreg fifocfg;
    u_fifoenreg  fifoen;
    u_tunestepreg tunestep;
    u_tunestepstdreg tunestepstd;
    u_tunestepddrreg tunestepddr;
    uint32_t reserved11;
    uint32_t reserved12;
    uint32_t reserved13;
    uint32_t reserved14;
    uint32_t reserved15;
    uint32_t reserved16;
    uint32_t reserved17;
    uint32_t reserved18;
    uint32_t reserved19;
    uint32_t reserved20;
    uint32_t reserved21;
    uint32_t reserved22;
    uint32_t reserved23;
    uint32_t reserved24;
    uint32_t reserved25;
    uint32_t reserved26;
    uint32_t reserved27;
    uint32_t reserved28;
    uint32_t reserved29;
    uint32_t reserved30;
    uint32_t reserved31;
    uint32_t reserved32;
    uint32_t reserved33;
    u_spiintreg spiint;
    uint32_t reserved34;
    uint32_t reserved35;
    u_slotisrreg slotisr;
} s_emmc;

typedef struct sdcmd
{
    uint32_t start  : 1;
    uint32_t trans  : 1;
    uint32_t cmdidx : 6;
    uint32_t arg    : 32;
    uint32_t crc7   : 7;
    uint32_t end    : 1;
} s_sdcmd;

typedef struct sdstatus
{
    uint32_t outofrange     : 1;
    uint32_t addr_err       : 1;
    uint32_t blklen_err     : 1;
    uint32_t erseq_err      : 1;
    uint32_t erparam        : 1;
    uint32_t wpviol         : 1;
    uint32_t cardlocked     : 1;
    uint32_t cardlockfailed : 1;
    uint32_t comcrc_err     : 1;
    uint32_t ill_cmd        : 1;
    uint32_t careccfailed   : 1;
    uint32_t cc_err         : 1;
    uint32_t error          : 1;
    uint32_t reserved1      : 1;
    uint32_t reserved2      : 1;
    uint32_t csd_over       : 1;
    uint32_t wperskip       : 1;
    uint32_t cardecc_dis    : 1;
    uint32_t erreset        : 1;
    uint32_t cur_state      : 4;
    uint32_t ready          : 1;
    uint32_t reserved3      : 1;
    uint32_t acmd           : 1;
    uint32_t reserved4      : 1;
    uint32_t akeseq_err     : 1;
} s_sdstatus;

typedef struct sdrep1
{
    uint32_t start    : 1;
    uint32_t trans    : 1;
    uint32_t cmdidx   : 6;
    uint32_t cardstat : 32;
    uint32_t crc7     : 7;
    uint32_t end      : 1;
} s_sdrep1;

typedef struct sdrep2
{
    uint32_t start    : 1;
    uint32_t trans    : 1;
    uint32_t reserved : 6;
    uint32_t cid[4];
    uint32_t end      : 1;
} s_sdrep2;

typedef struct sdrep3
{
    uint32_t start     : 1;
    uint32_t trans     : 1;
    uint32_t reserved  : 6;
    uint32_t ocr       : 32;
    uint32_t reserved2 : 8;
    uint32_t end       : 1;
} s_sdrep3;

typedef struct sdrep6
{
    uint32_t start     : 1;
    uint32_t trans     : 1;
    uint32_t cmdidx    : 6;
    uint32_t newpubrca : 16;
    uint32_t cardstat  : 16;
    uint32_t crc7      : 7;
    uint32_t end       : 1;
} s_sdrep6;

typedef struct sdrep7
{
    uint32_t start      : 1;
    uint32_t trans      : 1;
    uint32_t cmdidx     : 6;
    uint32_t reserved   : 20;
    uint32_t voltacc    : 4;
    uint32_t chpattecho : 8;
    uint32_t crc7       : 7;
    uint32_t end        : 1;
} s_sdrep7;

int init_emmc_driver(void);

#endif /* !EMMC_H */
