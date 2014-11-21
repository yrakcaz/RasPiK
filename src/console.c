#include "../include/console.h"

void init_console(void)
{
    console->x = 0;
    console->y = 0;
    console->margin = 10;
}

void clear_console(void)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            putpixel(i, j, BLACK);
}

int kwrite(const char *str, uint32_t size, uint32_t color)
{
    uint32_t i;
    for (i = 0; i < size && str[i]; i++)
    {
        if (console->x >= SCREEN_WIDTH)
        {
            console->x = 0;
            console->y += console->margin;
            drawchar(console->x += console->margin, console->y, str[i], color);
        }
        else if (str[i] == '\n')
        {
            console->x = 0;
            console->y += console->margin;
        }
        else
            drawchar(console->x += console->margin, console->y, str[i], color);
    }
    return i;
}
