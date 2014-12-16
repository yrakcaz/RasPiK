#include "mailbox.h"

void write_mailbox(uint8_t chan, uint32_t data)
{
    while (read_mmio(MAILBOX_BASE + MAILBOX_STATE) & MAILBOX_FULL)
        flush();
    dmb();
    write_mmio((MAILBOX_BASE + MAILBOX_WRITE), chan | (data & DATAMASK32));
}

uint32_t read_mailbox(uint8_t chan)
{
    uint32_t timeout = 0;
    uint32_t data = 0;

    while (1)
    {
        while (read_mmio(MAILBOX_BASE + MAILBOX_STATE) & MAILBOX_EMPTY)
        {
            flush();
            if (timeout++ >= MAILBOX_TIMEOUT)
                return ERROR32;
        }
        dmb();
        data = read_mmio(MAILBOX_BASE);
        dmb();

        if ((data & 0xF) == chan)
            break;
    }

    return (data & DATAMASK32);
}

uint32_t getpower_mailbox(uint32_t device)
{
    volatile uint32_t buff[256] __attribute__((aligned(16)));
    uint32_t size = 1;
    buff[size++] = 0;
    buff[size++] = POWERGET_TAG;
    buff[size++] = 0x8;
    buff[size++] = 0x4;
    buff[size++] = device;
    buff[size++] = 0;
    buff[size++] = 0;
    buff[0] = 4 * size;
    uint32_t addr = (uint32_t)&buff;
    addr -= KERNEL_START;
    write_mailbox(8, addr);
    read_mailbox(8);
    if (buff[1] != REP_SUCCESS)
        return -1;
    return buff[6];
}

int setpower_mailbox(uint32_t device, uint32_t state)
{
    volatile uint32_t buff[256] __attribute__((aligned(16)));
    uint32_t size = 1;
    buff[size++] = 0;
    buff[size++] = POWERSET_TAG;
    buff[size++] = 0x8;
    buff[size++] = 0x8;
    buff[size++] = device;
    buff[size++] = state;
    buff[size++] = 0;
    buff[0] = 4 * size;
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

uint32_t getclock_mailbox(uint32_t clock)
{
    volatile uint32_t buff[256] __attribute__((aligned(16)));
    uint32_t size = 1;
    buff[size++] = 0;
    buff[size++] = CLOCK_TAG;
    buff[size++] = 0x8;
    buff[size++] = 0x4;
    buff[size++] = clock;
    buff[size++] = 0;
    buff[size++] = 0;
    buff[0] = 4 * size;
    uint32_t addr = (uint32_t)&buff;
    addr -= KERNEL_START;
    write_mailbox(8, addr);
    read_mailbox(8);
    if (buff[1] != REP_SUCCESS)
        return -1;
    return buff[6];
}
