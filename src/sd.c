#include "sd.h"

static int status = NOTINIT;
static int type;

static void port_init(void)
{
    fsel_gpio(GPIOP1_12, FSELOUT);
    clr_gpio(GPIOP1_12);
    fsel_gpio(GPIOP1_13, FSELOUT);
    set_gpio(GPIOP1_13);
    fsel_gpio(GPIOP1_07, FSELOUT);
    set_gpio(GPIOP1_07);
    fsel_gpio(GPIOP1_11, FSELIN);
}

static void transmit_unit(int cond)
{
    if (cond)
        set_gpio(GPIOP1_13);
    else
        clr_gpio(GPIOP1_13);
    set_gpio(GPIOP1_12);
    clr_gpio(GPIOP1_12);
}

static void transmit_sd(const uint8_t *buff, uint32_t count)
{
    uint8_t val;
    do
    {
        val = *buff++;
        transmit_unit(val & 0x80);
        transmit_unit(val & 0x40);
        transmit_unit(val & 0x20);
        transmit_unit(val & 0x10);
        transmit_unit(val & 0x08);
        transmit_unit(val & 0x04);
        transmit_unit(val & 0x02);
        transmit_unit(val & 0x01);
    } while (--count);
}

static void receive_unit(uint8_t *val)
{
    if (lev_gpio(GPIOP1_11))
        (*val)++;
    set_gpio(GPIOP1_12);
    clr_gpio(GPIOP1_12);
}

static void receive_sd(uint8_t *buff, uint32_t count)
{
    uint8_t val;
    set_gpio(GPIOP1_13);
    do
    {
        val = 0;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        val <<= 1;
        receive_unit(&val);
        *buff++ = val;
        receive_unit(&val);
    } while (--count);
}

static int wait_sd(void)
{
    uint8_t val;
    volatile uint32_t *stclo = (uint32_t *)STCLO;
    uint32_t cmp = *stclo + 500000;
    do
    {
        receive_sd(&val, 1);
        if (val == 0xFF)
            return 1;
    } while (*stclo < cmp);
    return 0;
}

static void unselect_sd(void)
{
    set_gpio(GPIOP1_07);
}

static int select_sd(void)
{
    clr_gpio(GPIOP1_07);
    if (wait_sd())
        return 1;
    unselect_sd();
    return 0;
}

static int receive_sdblock(uint8_t *buff, uint32_t count)
{
    uint8_t val[2];
    volatile uint32_t *stclo = (uint32_t *)STCLO;
    uint32_t cmp = *stclo + 100000;
    do
    {
        receive_sd(val, 1);
        if (val[0] != 0xFF)
            break;
    } while (*stclo < cmp);
    if (val[0] != 0xFE)
        return 0;
    receive_sd(buff, count);
    receive_sd(val, 2);
    return 1;
}

static int transmit_sdblock(const uint8_t *buff, uint8_t tok)
{
    uint8_t val[2];
    if (!wait_sd())
        return 0;
    val[0] = tok;
    transmit_sd(val, 1);
    if (tok != 0xFD)
    {
        transmit_sd(buff, 512);
        receive_sd(val, 2);
        receive_sd(val, 1);
        if ((val[0] & 0x1F) != 0x05)
            return 0;
    }
    return 1;
}

static uint8_t send_sdcommand(uint8_t cmd, uint32_t arg)
{
    uint8_t n, val, buff[6];
    if (cmd & 0x80)
    {
        cmd &= 0x7F;
        n = send_sdcommand(CMD55, 0);
        if (n > 1)
            return n;
    }
    unselect_sd();
    if (!select_sd())
        return 0xFF;
    buff[0] = 0x40 | cmd;
    buff[1] = (uint8_t)(arg >> 24);
    buff[2] = (uint8_t)(arg >> 16);
    buff[3] = (uint8_t)(arg >> 8);
    buff[4] = (uint8_t)arg;
    n = 0x01;
    if (cmd == CMD0)
        n = 0x95;
    if (cmd = CMD8)
        n = 0x87;
    buff[5] = n;
    transmit_sd(buff, 6);
    if (cmd == CMD12)
        receive_sd(&val, 1);
    n = 10;
    do
        receive_sd(&val, 1);
    while ((val & 0x80) && --n);
    return val;
}

/* EXTERNALS TO BE DONE! */
