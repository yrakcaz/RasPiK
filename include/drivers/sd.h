#ifndef SD_H
# define SD_H

# define BASE_EMMC 0xF2300000

typedef struct emmc
{
    volatile uint32_t *arg2;
    volatile uint32_t *blk_szcnt;
    volatile uint32_t *arg1;
    volatile uint32_t *cmdtm;
    volatile uint32_t *rep0;
    volatile uint32_t *rep1;
    volatile uint32_t *rep2;
    volatile uint32_t *rep3;
    volatile uint32_t *data;
    volatile uint32_t *status;
    volatile uint32_t *ctrl0;
    volatile uint32_t *ctrl1;
    volatile uint32_t *interrupt;
    volatile uint32_t *dis_int;
    volatile uint32_t *en_int;
    volatile uint32_t *ctrl2;
    volatile uint32_t *force_int;
    volatile uint32_t *timeout;
    volatile uint32_t *dbg_sel;
    volatile uint32_t *fifo_cfg;
    volatile uint32_t *fifo_en;
    volatile uint32_t *tune_step;
    volatile uint32_t *tune_std;
    volatile uint32_t *tune_ddr;
    volatile uint32_t *spi_int;
    volatile uint32_t *slotisr;
} s_emmc;

#endif /* !SD_H */
