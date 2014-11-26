#include "gpio.h"

void set_gpio(uint8_t pin)
{
    gpio->set0 = (1 << pin);
}

void clr_gpio(uint8_t pin)
{
    gpio->clr0 = (1 << pin);
}

uint8_t lev_gpio(uint8_t pin)
{
    uint32_t val = gpio->lev0;
    return (val & (1 << pin)) ? HI : LO;
}

void gpio_write(uint8_t pin, uint8_t on)
{
    if (on)
        set_gpio(pin);
    else
        clr_gpio(pin);
}
