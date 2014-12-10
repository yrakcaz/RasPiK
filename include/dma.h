#ifndef DMA_H
# define DMA_H

# include <stdint.h>

# define BASE_DMA          0xF2007000

# define STATUS_DMA        (BASE_DMA + 0xFE0)
# define ENABLE_DMA        (BASE_DMA + 0xFF0)

# define EMMC_DMA          0
# define MAXCHAN_DMA       14

# define ACTIVE_DMA        (1 << 0)
# define END_DMA           (1 << 2)
# define INT_DMA           (1 << 2)
# define ERROR_DMA         (1 << 8)
# define RESET_DMA         (1 << 31)

# define INT_INFO_DMA      (1 << 0)
# define WAIT_INFO_DMA     (1 << 3)
# define DSTINC_INFO_DMA   (1 << 4)
# define DSTWIDTH_INFO_DMA (1 << 5)
# define DSTDREQ_INFO_DMA  (1 << 6)
# define SRCINC_INFO_DMA   (1 << 8)
# define SRCWIDTH_INFO_DMA (1 << 9)
# define SRCDREQ_INFO_DMA  (1 << 10)

# define PERM_INFO_DMA(X)  ((X) << 16)
# define EMMC_INFO_DMA(X)  PERM_INFO_DMA(11)

# define CS(X)             (volatile uint32_t *)(BASE_DMA + (X) * 0x100 + 0x00)
# define CBAD(X)           (volatile void **)(BASE_DMA + (X) * 0x100 + 0x04)

typedef struct dmablk
{
    uint32_t ti;
    void *src_ad;
    void *dst_ad;
    uint32_t txfr_len;
    uint32_t stride;
    void *next;
} s_dmablk;

void reset_dma(uint32_t channel);
int init_dma(uint32_t channel, s_dmablk *blk);

#endif /* !DMA_H */
