#include "../include/console.h"

static void clear_console(void)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            putpixel(i, j, BLACK);
    console->x = 0;
    console->y = 0;
}

void init_console(void)
{
    console->x = 0;
    console->y = 0;
    console->margin = MARGIN;
    clear_console();
}

static void scroll(void)
{
    for (int i = 0; i < NBCOL; i++)
        deletechar(console->x * CHAR_SIZE + console->margin, console->margin);
    for (int i = 0; i < NBCOL; i++)
    {
        for (int j = 1; j < NBLIN; j++)
        {
            replychar(i * CHAR_SIZE + console->margin,
                      j * FONT_SIZE + console->margin,
                      i * CHAR_SIZE + console->margin,
                      (j - 1) * FONT_SIZE + console->margin);
            deletechar(i * CHAR_SIZE + console->margin,
                       j * FONT_SIZE + console->margin);
        }
    }
}

int write_console(const char *str, uint32_t size, uint32_t color)
{
    int i;
    for (i = 0; i < size && str[i]; i++)
    {
        if (console->x >= NBCOL)
        {
            console->x = 0;
            if (console->y == NBLIN - 1)
                scroll();
            else
                console->y++;
        }

        switch (str[i])
        {
            case '\n':
                console->x = 0;
                if (console->y == NBLIN - 1)
                    scroll();
                else
                    console->y++;
                break;
            case '\t':
                if (console->x + 4 < NBCOL)
                    console->x += 4;
                break;
            case '\b':
                if (console->x > 0)
                {
                    console->x--;
                    deletechar(console->x * CHAR_SIZE + console->margin, 
                               console->y * FONT_SIZE + console->margin);
                }
                break;
            default:
                drawchar(console->x++ * CHAR_SIZE + console-> margin, 
                         console->y * FONT_SIZE + console->margin,
                         str[i], color);
                break;
        }
    }
    return i;
}
