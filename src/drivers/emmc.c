#include "drivers/emmc.h"

/*static */uint32_t basefreq_sd(void)
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
