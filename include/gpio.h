#ifndef GPIO_H
# define GPIO_H

# include "uart.h"

# define GPIOP1_03 2
# define GPIOP1_05 3
# define GPIOP1_07 4
# define GPIOP1_08 14
# define GPIOP1_10 15
# define GPIOP1_11 17
# define GPIOP1_12 18
# define GPIOP1_13 27
# define GPIOP1_15 22
# define GPIOP1_16 23
# define GPIOP1_18 24
# define GPIOP1_19 10
# define GPIOP1_21 9
# define GPIOP1_22 25
# define GPIOP1_23 11
# define GPIOP1_24 8
# define GPIOP1_26 7

# define STCLO  0x0004
# define FSELIN 0b000
# define FSELOU 0b001

typedef struct gpio
{
    uint32_t fsel0;
    uint32_t fsel1;
    uint32_t fsel2;
    uint32_t fsel3;
    uint32_t fsel4;
    uint32_t fsel5;
    uint32_t res1;
    uint32_t set0;
    uint32_t set1;
    uint32_t res2;
    uint32_t clr0;
    uint32_t clr1;
    uint32_t res3;
    uint32_t lev0;
    uint32_t lev1;
    uint32_t res4;
    uint32_t eds[2];
    uint32_t res5;
    uint32_t ren[2];
    uint32_t res6;
    uint32_t fen[2];
    uint32_t res7;
    uint32_t hen[2];
    uint32_t res8;
    uint32_t len[2];
    uint32_t res9;
    uint32_t aren[2];
    uint32_t res10;
    uint32_t afen[2];
    uint32_t res11;
    uint32_t pud;
    uint32_t pudclk0;
    uint32_t pudclk1;
} s_gpio;

s_gpio *gpio = (s_gpio *)BASE_GPIO;

extern void fsel_gpio(uint8_t pin, uint8_t mode);
void set_gpio(uint8_t pin);
void clr_gpio(uint8_t pin);
void lev_gpio(uint8_t pin);
void gpio_write(uint8_t pin, uint8_t on);

#endif /* !GPIO_H */
