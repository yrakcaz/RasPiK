#ifndef CONSOLE_H
# define CONSOLE_H

# include "uart.h"
# include "graphics.h"

/* Usefull defines */
# define NBCOL  97
# define NBLIN  36
# define MARGIN 10

/* Structure which describes the console */
typedef struct console
{
    uint32_t x;
    uint32_t y;
    uint32_t margin;
} s_console;

/* External functions... */
void init_console(void);
int write_console(const char *str, uint32_t size, uint32_t color);

#endif /* !CONSOLE_H */
