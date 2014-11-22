#ifndef CONSOLE_H
# define CONSOLE_H

# include "graphics.h"

# define NBCOL  97
# define NBLIN  36
# define MARGIN 10

typedef struct console
{
    uint32_t x;
    uint32_t y;
    uint32_t margin;
} s_console;

s_console *console;

void init_console(void);
int kwrite(const char *str, uint32_t size, uint32_t color);

#endif /* !CONSOLE_H */
