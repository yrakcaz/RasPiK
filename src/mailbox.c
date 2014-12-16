#include "mailbox.h"

void write_mailbox(uint8_t chan, uint32_t data)
{
    while (read_mmio(MAILBOX_BASE + MAILBOX_STATE) & MAILBOX_FULL) {}
    sync_mem();
    write_mmio((MAILBOX_BASE + MAILBOX_WRITE), chan | (data & DATAMASK32));
    sync_mem();
}

uint32_t verify_mailbox(uint8_t chan)
{
    uint32_t timeout = 0;
    uint32_t data = 0;

    while (1)
    {
        while (read_mmio(MAILBOX_BASE + MAILBOX_STATE) & MAILBOX_EMPTY)
            if (timeout++ >= MAILBOX_TIMEOUT)
                return ERROR32;

        sync_mem();
        data = read_mmio(MAILBOX_BASE);
        sync_mem();

        if ((data & 0xF) == chan)
            break;
    }

    return (data & DATAMASK32);
}

