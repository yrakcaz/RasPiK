#include "drivers/emmc.h"

static volatile s_emmc *emmc;
static s_sd device;
static uint32_t usedma;
static uint32_t hciversion;
static uint32_t sdcap0;
static uint32_t sdcap1;

static uint32_t basefreq_sd(void)
{
    volatile uint32_t buff[256] __attribute__((aligned(16)));
    buff[0] = 8 * sizeof (uint32_t);
    buff[1] = 0;
    buff[2] = CLOCK_TAG;
    buff[3] = 8;
    buff[4] = 4;
    buff[5] = 1;
    buff[6] = 0;
    buff[7] = 0;
    uint32_t addr = (uint32_t)&buff;
    addr -= KERNEL_START;
    write_mailbox(8, addr);
    read_mailbox(8);
    if (buff[1] != REP_SUCCESS || buff[5] != 0x1)
        return -1;
    return buff[6];
}

/*static */int power_emmc(uint32_t state)
{
    volatile uint32_t buff[256] __attribute__((aligned(16)));
    buff[0] = 8 * sizeof (uint32_t);
    buff[1] = 0;
    buff[2] = POWERSET_TAG;
    buff[3] = 0x8;
    buff[4] = 0x8;
    buff[5] = EMMC_CHAN;
    buff[6] = state;
    buff[7] = 0;
    uint32_t addr = (uint32_t)&buff;
    addr -= KERNEL_START;
    write_mailbox(8, addr);
    wait(200);
    read_mailbox(8);
    if (buff[1] != REP_SUCCESS || buff[5] ||
        (buff[6] & 0x3) != state)
        return -1;
    return 0;
}

static uint32_t getclkdiv(uint32_t base, uint32_t rate)
{
    uint32_t tgtdiv = 0;
    if (rate > base)
        tgtdiv = 1;
    else
    {
        tgtdiv = base / rate;
        if (base % rate)
            tgtdiv--;
    }

    uint32_t div = -1;
    uint32_t first;
    for (first = 31; first != 0; first--)
    {
        uint32_t test = (1 << first);
        if (tgtdiv & test)
        {
            div = first;
            tgtdiv &= ~test;
            if (tgtdiv)
                div++;
            break;
        }
    }

    if (div == -1)
        div = 31;
    if (div >= 32)
        div = 31;
    if (div)
        div = (1 << (div - 1));
    if (div >= 0x400)
        div = 0x3FF;

    uint32_t frqsel = div & 0xFF;
    uint32_t upper = (div >> 8) & 0x3;
    return ((frqsel << 8) | (upper << 6) | (0 << 5));
}

int init_emmc(s_device *dev)
{
    emmc = (s_emmc *)dev->addr;
    device.rca = 0;
    device.blks_tr = 0;
    usedma = 0;
    hciversion = (emmc->slotisr.raw >> 16) & 0xFF;
    if (hciversion < 2)
        return -1;

    uint32_t ctrl1 = emmc->ctrl1.raw;
    ctrl1 |= (1 << 24);
    ctrl1 &= ~(1 << 2);
    ctrl1 &= ~(1 << 0);
    emmc->ctrl1.raw = ctrl1;

    while (emmc->ctrl1.raw & (0x7 << 24)) {}

    sdcap0 = emmc->cap0;
    sdcap1 = emmc->cap1;

    while ((emmc->status.raw & (1 << 16)) != (1 << 16)) {}

    emmc->ctrl2.raw = 0;

    uint32_t baseclk = basefreq_sd();
    if (baseclk == -1)
        baseclk = 100000000;

    ctrl1 = emmc->ctrl1.raw;
    ctrl1 |= 1;

    uint32_t id_freq = getclkdiv(baseclk, SDCLK_ID);
    if (id_freq == -1)
        return -1;

    //TODO (base : 330)

    return 0;
}

int read_emmc(s_device *device, uint32_t *blk_num, void *buff, uint32_t len);
