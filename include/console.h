#ifndef CONSOLE_H
# define CONSOLE_H

# include "graphics.h"

# define BLACK 0x00000000
# define WHITE 0xFFFFFFFF

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
