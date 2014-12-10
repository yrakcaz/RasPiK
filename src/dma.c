#include "dma.h"

void reset_dma(uint32_t channel)
{
    if (channel > MAXCHAN_DMA)
        return;
    *CS(channel) = RESET_DMA;
    while (*CS(channel) & RESET_DMA) {}
}

int init_dma(uint32_t channel, s_dmablk *blk)
{
    if (channel > MAXCHAN_DMA)
        return 1;

    blk->ti |= INT_INFO_DMA;
    blk->next = 0;

    asm volatile ("mcr p15, 0, %0, c7, c10, 4"
                  :: "r"(0) : "memory");

    *CBAD(channel) = blk;
    *CS(channel) = ACTIVE_DMA | END_DMA | INT_DMA;

    while (!(*CS(channel) & (INT_DMA | ERROR_DMA))) {}
    if (*CS(channel) & ERROR_DMA)
        return 2;

    return 0;
}
