#include "gpio.h"

void set_gpio(uint8_t pin)
{
    gpio->set0 = (1 << pin);
}

void clr_gpio(uint8_t pin)
{
    gpio->clr0 = (1 << pin);
}

void lev_gpio(uint8_t pin)
{
    gpio->lev0 = (1 << pin);
}

void gpio_write(uint8_t pin, uint8_t on)
{
    if (on)
        set_gpio(pin);
    else
        clr_gpio(pin);
}
