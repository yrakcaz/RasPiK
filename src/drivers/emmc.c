#include "drivers/emmc.h"

static volatile s_emmc *emmc;
static s_sd device;
static uint32_t usedma;
static uint32_t hciversion;
static uint32_t sdcap0;
static uint32_t sdcap1;
static int lastcmd = 0;
static int lasterr = 0;

static int ACMD[] =
{
    SDCMD_RES(0),
    SDCMD_RES(1),
    SDCMD_RES(2),
    SDCMD_RES(3),
    SDCMD_RES(4),
    SDCMD_RES(5),
    SDCMD_INDEX(6) | SD_REPR1 | APP_CMD,
    SDCMD_RES(7),
    SDCMD_RES(8),
    SDCMD_RES(9),
    SDCMD_RES(10),
    SDCMD_RES(11),
    SDCMD_RES(12),
    SDCMD_INDEX(13) | SD_REPR1 | APP_CMD,
    SDCMD_RES(14),
    SDCMD_RES(15),
    SDCMD_RES(16),
    SDCMD_RES(17),
    SDCMD_RES(18),
    SDCMD_RES(19),
    SDCMD_RES(20),
    SDCMD_RES(21),
    SDCMD_INDEX(22) | SD_REPR1 | SD_READ | APP_CMD,
    SDCMD_INDEX(23) | SD_REPR1 | APP_CMD,
    SDCMD_RES(24),
    SDCMD_RES(25),
    SDCMD_RES(26),
    SDCMD_RES(27),
    SDCMD_RES(28),
    SDCMD_RES(29),
    SDCMD_RES(30),
    SDCMD_RES(31),
    SDCMD_RES(32),
    SDCMD_RES(33),
    SDCMD_RES(34),
    SDCMD_RES(35),
    SDCMD_RES(36),
    SDCMD_RES(37),
    SDCMD_RES(38),
    SDCMD_RES(39),
    SDCMD_RES(40),
    SDCMD_INDEX(41) | SD_REPR3 | APP_CMD,
    SDCMD_INDEX(42) | SD_REPR1 | APP_CMD,
    SDCMD_RES(43),
    SDCMD_RES(44),
    SDCMD_RES(45),
    SDCMD_RES(46),
    SDCMD_RES(47),
    SDCMD_RES(48),
    SDCMD_RES(49),
    SDCMD_RES(50),
    SDCMD_INDEX(51) | SD_REPR1 | SD_READ | APP_CMD
};

static int CMD[] =
{
    SDCMD_INDEX(0)  | SD_REPNONE,
    SDCMD_RES(1),
    SDCMD_INDEX(2)  | SD_REPR2,
    SDCMD_INDEX(3)  | SD_REPR6,
    SDCMD_INDEX(4)  | SD_REPNONE,
    SDCMD_INDEX(5)  | SD_REPR4,
    SDCMD_INDEX(6)  | SD_REPR1,
    SDCMD_INDEX(7)  | SD_REPR1B,
    SDCMD_INDEX(8)  | SD_REPR7,
    SDCMD_INDEX(9)  | SD_REPR2,
    SDCMD_INDEX(10) | SD_REPR2,
    SDCMD_INDEX(11) | SD_REPR1,
    SDCMD_INDEX(12) | SD_REPR1B | SDCMD_ABORT,
    SDCMD_INDEX(13) | SD_REPR1,
    SDCMD_RES(14),
    SDCMD_INDEX(15) | SD_REPNONE,
    SDCMD_INDEX(16) | SD_REPR1,
    SDCMD_INDEX(17) | SD_REPR1 | SD_READ,
    SDCMD_INDEX(18) | SD_REPR1 | SD_READ | SDCMD_MULBLK,
    SDCMD_INDEX(19) | SD_REPR1 | SD_READ,
    SDCMD_INDEX(20) | SD_REPR1B,
    SDCMD_RES(21),
    SDCMD_RES(22),
    SDCMD_INDEX(23) | SD_REPR1,
    SDCMD_INDEX(24) | SD_REPR1 | SD_WRITE,
    SDCMD_INDEX(25) | SD_REPR1 | SD_WRITE | SDCMD_MULBLK,
    SDCMD_RES(26),
    SDCMD_INDEX(27) | SD_REPR1 | SD_WRITE,
    SDCMD_INDEX(28) | SD_REPR1B,
    SDCMD_INDEX(29) | SD_REPR1B,
    SDCMD_INDEX(30) | SD_REPR1B | SD_READ,
    SDCMD_RES(31),
    SDCMD_INDEX(32) | SD_REPR1,
    SDCMD_INDEX(33) | SD_REPR1,
    SDCMD_RES(34),
    SDCMD_RES(35),
    SDCMD_RES(36),
    SDCMD_RES(37),
    SDCMD_INDEX(38) | SD_REPR1B,
    SDCMD_RES(39),
    SDCMD_RES(40),
    SDCMD_RES(41),
    SDCMD_INDEX(42) | SD_REPR1,
    SDCMD_RES(43),
    SDCMD_RES(44),
    SDCMD_RES(45),
    SDCMD_RES(46),
    SDCMD_RES(47),
    SDCMD_RES(48),
    SDCMD_RES(49),
    SDCMD_RES(50),
    SDCMD_RES(51),
    SDCMD_RES(52),
    SDCMD_RES(53),
    SDCMD_RES(54),
    SDCMD_INDEX(55) | SD_REPR1,
    SDCMD_INDEX(56) | SD_REPR1 | SDCMD_ISDATA
};

static void waitms(uint32_t ms)
{
    wait(ms * 1000);
}

static uint32_t byte_swap(unsigned int in)
{
    uint32_t b0 = in & 0xFF;
    uint32_t b1 = (in >> 8) & 0xFF;
    uint32_t b2 = (in >> 16) & 0xFF;
    uint32_t b3 = (in >> 24) & 0xFF;
    uint32_t ret = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    return ret;
}

static void write_word(uint32_t val, uint8_t* buf, int offset)
{
    buf[offset + 0] = val & 0xff;
    buf[offset + 1] = (val >> 8) & 0xff;
    buf[offset + 2] = (val >> 16) & 0xff;
    buf[offset + 3] = (val >> 24) & 0xff;
}

static uint32_t read_word(uint8_t* buf, int offset)
{
    uint32_t b0 = buf[offset + 0] & 0xff;
    uint32_t b1 = buf[offset + 1] & 0xff;
    uint32_t b2 = buf[offset + 2] & 0xff;
    uint32_t b3 = buf[offset + 3] & 0xff;

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

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

static int power_emmc(uint32_t state)
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
    waitms(200);
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

static uint32_t switchclkrate(uint32_t base, uint32_t rate)
{
    uint32_t div = getclkdiv(base, rate);
    if (div == -1)
        return -1;

    while (emmc->status.bits.cmd_inhibit == 1 ||
           emmc->status.bits.dat_inhibit == 1)
        waitms(10);

    emmc->ctrl1.raw &= ~(1 << 2);
    waitms(20);

    uint32_t ctrl1 = emmc->ctrl1.raw;
    ctrl1 &= ~0xFFE0;
    ctrl1 |= div;
    emmc->ctrl1.raw = ctrl1;
    waitms(20);

    ctrl1 |= (1 << 2);
    emmc->ctrl1.raw = ctrl1;
    waitms(20);

    return 1;
}

static int issuecmdint(uint32_t cmd, uint32_t arg)
{
    while (emmc->status.raw & 0x1)
        waitms(5);

    if ((cmd & SDCMD_REPMSK) == SDCMD_REP48B)
        if ((cmd & SDCMD_REPMSK) != SDCMD_ABORT)
            while (emmc->status.raw & 0x2)
                waitms(10);

    uint32_t sdma = 0;
    if ((cmd & SDCMD_ISDATA) && usedma)
    {
        sdma = 1;
        emmc->arg2 = SDMA_BUFFPA;
    }

    if (device.blks_tr > 0xFFFF)
        return 0;

    uint32_t blkszcnt = device.blksz | (device.blks_tr << 16);
    emmc->blkszcnt.raw = blkszcnt;

    emmc->arg1 = arg;

    if (sdma)
        cmd |= SDCMD_DMA;
    emmc->cmdtm.raw = cmd;
    waitms(5);

    while (!(emmc->interrupt.bits.cmd_done) && !(emmc->interrupt.bits.error)) {}

    uint32_t irpt = emmc->interrupt.raw;
    emmc->interrupt.raw = 0xFFFF0001;
    if ((irpt & 0xFFFF0001) != 0x1)
    {
        lasterr = irpt & 0xFFFF0000;
        return 0;
    }
    waitms(5);

    switch(cmd & SDCMD_REPMSK)
    {
        case SDCMD_REP48:
        case SDCMD_REP48B:
            device.rep0 = emmc->rep0;
            break;
        case SDCMD_REP136:
            device.rep0 = emmc->rep0;
            device.rep1 = emmc->rep1;
            device.rep2 = emmc->rep2;
            device.rep3 = emmc->rep3;
            break;
    }

    if ((cmd & SDCMD_ISDATA) && !sdma)
    {
        uint32_t wrint;
        int iswrite = 0;
        if (cmd & SDCMD_DDCH)
            wrint = (1 << 5);
        else
        {
            iswrite = 1;
            wrint = (1 << 4);
        }

        int curblk = 0;
        uint32_t *curbufaddr = device.rcv_buf;
        while (curblk < device.blks_tr)
        {
            while (!(emmc->interrupt.raw & (wrint | 0x8000))) {}
            irpt = emmc->interrupt.raw;
            emmc->interrupt.raw = 0xFFFF0000 | wrint;
            if ((irpt & (0xFFFF0000 | wrint)) != wrint)
            {
                lasterr = irpt & 0xFFFF0000;
                return 0;
            }

            uint32_t curbytenum = 0;
            while (curbytenum < device.blksz)
            {
                if (iswrite)
                {
                    uint32_t data = read_word((uint8_t *)curbufaddr, 0);
                    emmc->data = data;
                }
                else
                {
                    uint32_t data = emmc->data;
                    write_word(data, (uint8_t *)curbufaddr, 0);
                }
                curbytenum += 4;
                curbufaddr++;
            }
            curblk++;
        }
    }

    if ((((cmd & SDCMD_REPMSK) == SDCMD_REP48B) ||
                (cmd & SDCMD_ISDATA)) & (sdma))
    {
        if (!(emmc->status.raw & 0x2))
            emmc->interrupt.raw = 0xFFFF0002;
        else
        {
            while (!(emmc->interrupt.raw & 0x8002)) {}
            irpt = emmc->interrupt.raw;
            emmc->interrupt.raw = 0xFFFF0002;
            if (((irpt & 0xFFFF0002) != 0x2) && ((irpt & 0xFFFF0002) != 0x100002))
            {
                lasterr = irpt & 0xFFFF0000;
                return 0;
            }
            emmc->interrupt.raw = 0xFFFF0002;
        }
    }
    else if (sdma)
    {
        if (!(emmc->status.raw & 0x2))
            emmc->interrupt.raw = 0xFFFF000A;
        else
        {
            while (!(emmc->interrupt.raw & 0x800A)) {}
            irpt = emmc->interrupt.raw;
            emmc->interrupt.raw = 0xFFFF000A;

            if ((irpt & 0x8000) && ((irpt & 0x2) != 0x2))
            {
                lasterr = irpt & 0xFFFF0000;
                return 0;
            }
            if ((irpt & 0x8) && ((irpt & 0x2) != 0x2))
            {
                lasterr = irpt & 0xFFFF0000;
                return 0;
            }

            if (irpt & 0x2)
                for (int i = 0; i < device.blksz; i++)
                    ((char *)device.rcv_buf)[i] = ((char *)SDMA_BUFF)[i];
            else
            {
                if (!irpt && ((emmc->status.raw & 0x3) == 0x2))
                {
                    emmc->cmdtm.raw = CMD[EMMC_STOPTR];
                    waitms(200);
                }
                lasterr = irpt & 0xFFFF0000;
                return 0;
            }
        }
    }
    return 1;
}

void handleemmcint(void);

static int issuecmd(uint32_t cmd, uint32_t arg)
{
    handleemmcint();

    if (cmd & APP_CMD)
    {
        cmd &= 0xFF;
        if (ACMD[cmd] == SDCMD_RES(0))
            return 0;
        lastcmd = EMMC_ACMD;

        uint32_t rca = 0;
        if (device.rca)
            rca = (device.rca << 16);
        if (issuecmdint(CMD[EMMC_ACMD], rca))
        {
            lastcmd = cmd | APP_CMD;
            issuecmdint(ACMD[cmd], arg);
        }
    }
    else
    {
        if (CMD[cmd] == SDCMD_RES(0))
            return 0;
        lastcmd = cmd;
        issuecmdint(CMD[cmd], arg);
    }

    return 1;
}

void handleemmcint(void)
{
    uint32_t irpt = emmc->interrupt.raw;
    uint32_t rstmsk = 0;
    if (irpt & (1 << 0))
        rstmsk |= (1 << 0);
    if (irpt & (1 << 1))
        rstmsk |= (1 << 1);
    if (irpt & (1 << 2))
        rstmsk |= (1 << 2);
    if (irpt & (1 << 3))
        rstmsk |= (1 << 3);
    if (irpt & (1 << 4))
        rstmsk |= (1 << 4);
    if (irpt & (1 << 5))
        rstmsk |= (1 << 5);
    if (irpt & (1 << 6))
        rstmsk |= (1 << 6);
    if (irpt & (1 << 7))
        rstmsk |= (1 << 7);
    if (irpt & (1 << 8))
    {
        if (device.rca)
            issuecmd(EMMC_SENDSTAT, (device.rca << 16));
        rstmsk = (1 << 8);
    }
    if (irpt & 0x8000)
        rstmsk |= 0xFFFF8000;
    emmc->interrupt.raw = rstmsk;
}

static int vswitchfailed(s_device *dev)
{
    device.failed = 1;
    power_emmc(OFF);
    return dev->driver->init(dev);
}

static void setversion_sd(void)
{
    uint32_t scr0 = byte_swap(device.scr.scr[0]);
    uint32_t sdspec = (scr0 >> (56 - 32)) & 0xF;
    uint32_t sdspec3 = (scr0 >> (47 - 32)) & 0x1;
    uint32_t sdspec4 = (scr0 >> (42 - 32)) & 0x1;
    device.scr.bus_widths = (scr0 >> (48-32)) & 0xF;
    device.scr.version = SD_UNKNOWN;

    if (!sdspec)
        device.scr.version = SD_V1;
    else if (sdspec == 1)
        device.scr.version = SD_V1_1;
    else if (sdspec == 2)
    {
        if (!sdspec3)
            device.scr.version = SD_V2;
        else if (sdspec3 == 1)
        {
            if (!sdspec4)
                device.scr.version = SD_V3;
            else if (sdspec4 == 1)
                device.scr.version = SD_V4;
        }
    }
}

static int init_emmc(s_device *dev)
{
    emmc = (s_emmc *)(dev->addr);
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

    ctrl1 |= id_freq;
    ctrl1 |= (7 << 16);

    emmc->ctrl1.raw = ctrl1;

    while (emmc->ctrl1.raw & 0x2) {}

    waitms(10);
    emmc->ctrl1.raw |= 4;
    waitms(10);

    emmc->inten.raw = 0;
    emmc->interrupt.raw = 0xFFFFFFFF;
    uint32_t intmsk = 0xFFFFFFFF & (~SDC_INT);
    intmsk |= SDC_INT;
    emmc->intmsk.raw = intmsk;
    waitms(10);

    if (!issuecmd(EMMC_IDLE, 0))
        return -1;
    int v2 = 0;
    if (!issuecmd(EMMC_SENDIF, 0x1AA))
        return -1;
    if ((device.rep0 & 0xFFF) != 0x1AA)
        return -1;
    v2 = 1;
    if (!issuecmd(ACMD(41), 0))
        return -1;

    uint32_t csupvswitch = 0;
    while (1)
    {
        uint32_t flags = 0;
        if (v2)
        {
            flags |= (1 << 30);
            if (!device.failed)
                flags |= (1 << 24);
            flags |= (1 << 28);
        }

        if (!issuecmd(ACMD(41), 0x00FF8000 | flags))
            return -1;

        if ((device.rep0 >> 31) & 0x1)
        {
            device.ocr = (device.rep0 >> 8) & 0xFFFF;
            device.sup_sdhc = (device.rep0 >> 30) & 0x1;
            csupvswitch = (device.rep0 >> 24) & 0x1;
            break;
        }
        waitms(50);
    }

    switchclkrate(baseclk, SDCLK_NORMAL);
    waitms(20);

    if (csupvswitch)
    {
        if (!issuecmd(EMMC_SWVOLT, 0))
            return vswitchfailed(dev);

        emmc->ctrl1.raw &= ~(1 << 2);

        uint32_t streg = emmc->status.raw;
        uint32_t dat30 = (streg >> 20) & 0xF;
        if (dat30)
            return vswitchfailed(dev);

        emmc->ctrl0.raw |= (1 << 8);
        waitms(20);
        if (!((emmc->ctrl0.raw >> 8) & 0x1))
            return vswitchfailed(dev);

        emmc->ctrl1.raw |= (1 << 2);
        waitms(10);
        streg = emmc->status.raw;
        dat30 = (streg >> 20) & 0xF;
        if (dat30 != 0xF)
            return vswitchfailed(dev);
    }

    if (!issuecmd(EMMC_ALLCID, 0x0))
        return -1;

    device.cid[0] = device.rep0;
    device.cid[1] = device.rep1;
    device.cid[2] = device.rep2;
    device.cid[3] = device.rep3;

    if (!issuecmd(EMMC_RELADDR, 0))
        return -1;

    uint32_t cmd3rep = device.rep0;
    device.rca = (cmd3rep >> 16) & 0xFFFF;
    uint32_t crcerr = (cmd3rep >> 15) & 0x1;
    uint32_t illcmd = (cmd3rep >> 14) & 0x1;
    uint32_t error = (cmd3rep >> 13) & 0x1;
    uint32_t status = (cmd3rep >> 9) & 0xF;
    uint32_t ready = (cmd3rep >> 8) & 0x1;
    if (crcerr || illcmd || error || !ready)
        return -1;
    if (!issuecmd(EMMC_SELCARD, device.rca << 16))
        return -1;

    uint32_t cmd7rep = device.rep0;
    status = (cmd7rep >> 9) & 0xF;
    if ((status != 3) && (status != 4))
        return -1;

    if (!device.sup_sdhc)
        if (!issuecmd(EMMC_BLKLEN, 512))
            return -1;

    device.blksz = 512;
    uint32_t ctrl_blksz = emmc->blkszcnt.raw;
    ctrl_blksz &= (~0xFFF);
    ctrl_blksz |= 0x200;
    emmc->blkszcnt.raw = ctrl_blksz;

    device.rcv_buf = (uint32_t *)&(device.scr.scr[0]);
    device.blksz = 9;
    device.blks_tr = 1;
    if (!issuecmd(ACMD(51), 0))
        return -1;

    device.blksz = 512;

    setversion_sd();

    if (device.scr.bus_widths != 5)
    {
        uint32_t oldintmsk = emmc->intmsk.raw;
        uint32_t newintmsk = oldintmsk & ~(1 << 8);
        emmc->intmsk.raw = newintmsk;

        if (!issuecmd(ACMD(6), 0x2))
        {
            emmc->ctrl0.raw |= 0x2;
            emmc->intmsk.raw = oldintmsk;
        }
    }

    emmc->interrupt.raw = 0xFFFFFFFF;

    return 0;
}

static int ensuredatamode(s_device *dev)
{
    if (!device.rca)
    {
        int ret = dev->driver->init(dev);
        if (ret)
            return ret;
    }
    if (!issuecmd(EMMC_SENDSTAT, device.rca << 16))
    {
        device.rca = 0;
        return -1;
    }

    uint32_t status = device.rep0;
    uint32_t curst = (status >> 9) & 0xF;
    if (curst == 3)
    {
        if (!issuecmd(EMMC_SELCARD, device.rca << 16))
        {
            device.rca = 0;
            return -1;
        }
    }
    else if (curst == 5)
    {
        if (!issuecmd(EMMC_STOPTR, 0))
        {
            device.rca = 0;
            return -1;
        }
        emmc->ctrl1.raw |= (1 << 26);
        while ((emmc->ctrl1.raw & (1 << 26)) == 1) {}
    }
    else if (curst != 4)
    {
        int ret = dev->driver->init(dev);
        if (ret)
            return ret;
    }

    if (curst != 4)
    {
        if (issuecmd(EMMC_SENDSTAT, device.rca << 16))
        {
            device.rca = 0;
            return -1;
        }
        status = device.rep0;
        curst = (status >> 9) & 0xF;
        if (curst != 4)
        {
            device.rca = 0;
            return -1;
        }
    }

    return 0;
}

static uint32_t dodatacmdread(char *buf, uint32_t len, uint32_t blknum)
{
    if (!device.sup_sdhc)
        blknum *= 512;
    if (len < device.blksz)
        return -1;

    device.blks_tr = len / device.blksz;
    if (len % device.blksz)
        return -1;

    device.rcv_buf = (uint32_t *)buf;

    int cmd;
    cmd = device.blks_tr > 1 ? EMMC_RXBLK : EMMC_R1BLK;

    int retrycnt = 0;
    int maxretry = 3;
    while (retrycnt < maxretry)
    {
        if (issuecmd(cmd, blknum))
            break;
        else
            retrycnt++;
    }

    if (retrycnt == maxretry)
    {
        device.rca = 0;
        return -1;
    }

    return 0;
}

static uint32_t dodatacmdwrite(const char *buf, uint32_t len, uint32_t blknum)
{
    if (!device.sup_sdhc)
        blknum *= 512;
    if (len < device.blksz)
        return -1;

    device.blks_tr = len / device.blksz;
    if (len % device.blksz)
        return -1;

    device.rcv_buf = (uint32_t *)buf;

    int cmd;
    cmd = device.blks_tr > 1 ? EMMC_WXBLK : EMMC_W1BLK;

    int retrycnt = 0;
    int maxretry = 3;
    while (retrycnt < maxretry)
    {
        if (issuecmd(cmd, blknum))
            break;
        else
            retrycnt++;
    }

    if (retrycnt == maxretry)
    {
        device.rca = 0;
        return -1;
    }

    return 0;
}

static int read_emmc(s_device *dev, uint32_t *blknum, void *buff, uint32_t len)
{
    if (ensuredatamode(dev))
        return -1;
    if (dodatacmdread(buff, len, *blknum) == -1)
        return -1;
    return len;
}

static int write_emmc(s_device *dev, uint32_t *blknum, const void *buff, uint32_t len)
{
    if (ensuredatamode(dev))
        return -1;
    if (dodatacmdwrite(buff, len, *blknum) == -1)
        return -1;
    return len;
}

static int ioctl_emmc(s_device *dev, int cmd, int args)
{
    DO_NOTHING_WITH(dev);
    return issuecmd(cmd, args);
}

int init_emmc_driver(void)
{
    s_driver *driver = kmalloc(sizeof (s_driver));
    if (!driver)
        return 0;

    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    driver->init = init_emmc;
    driver->write = write_emmc;
    driver->read = read_emmc;
    driver->ioctl = ioctl_emmc;

    int ret = insmod("/dev/sdcard", (void *)EMMC_BASE, driver);
    wait(HUMAN_TIME / 2);

    if (ret < 0)
    {
        klog("\b\b\b\bKO", 6, RED);
        klog("]\tEMMC driver initialization failed.\n", 37, WHITE);
    }
    else
    {
        klog("\b\b\b\bOK", 6, GREEN);
        klog("]\tEMMC driver initialized!\n", 27, WHITE);
    }

    return ret;
}
