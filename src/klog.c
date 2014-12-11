#include "klog.h"

static s_console csl;

void clear_klogs(void)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            putpixel(i, j, BLACK);
    csl.x = 0;
    csl.y = 0;
}

static void kikoo_thing(void)
{
    klog("Kernel booting ", 15, RED);
    for (int i = 1; i <= 11; i++)
    {
        if (i % 4 == 0)
            klog("\b\b\b", 3, RED);
        else
            klog(".", 1, RED);
        for (int j = HUMAN_TIME * KIKOO_RATE; j; j--);
    }
    klog("\n\n\n", 3, RED);
    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);
    for (int j = HUMAN_TIME * KIKOO_RATE; j; j--);
    klog("\b\b\b\bOK", 6, GREEN);
    klog("]\tKlogs initialized!\n", 21, WHITE);
}

void init_klog(void)
{
    csl.x = 0;
    csl.y = 0;
    csl.margin = MARGIN;
    clear_klogs();
    kikoo_thing();
}

static void scroll(void)
{
    for (int i = 0; i < NBCOL; i++)
        deletechar(csl.x * CHAR_SIZE + csl.margin, csl.margin);
    for (int i = 0; i < NBCOL; i++)
    {
        for (int j = 1; j < NBLIN; j++)
        {
            replychar(i * CHAR_SIZE + csl.margin,
                      j * FONT_SIZE + csl.margin,
                      i * CHAR_SIZE + csl.margin,
                      (j - 1) * FONT_SIZE + csl.margin);
            deletechar(i * CHAR_SIZE + csl.margin,
                       j * FONT_SIZE + csl.margin);
        }
    }
}

int klog(const char *str, uint32_t size, uint32_t color)
{
    int i;
    for (i = 0; i < size && str[i]; i++)
    {
        if (csl.x >= NBCOL)
        {
            csl.x = 0;
            if (csl.y == NBLIN - 1)
                scroll();
            else
                csl.y++;
        }

        switch (str[i])
        {
            case '\n':
                csl.x = 0;
                if (csl.y == NBLIN - 1)
                    scroll();
                else
                    csl.y++;
                break;
            case '\t':
                if (csl.x + 4 < NBCOL)
                    csl.x += 4;
                break;
            case '\b':
                if (csl.x > 0)
                {
                    csl.x--;
                    deletechar(csl.x * CHAR_SIZE + csl.margin,
                               csl.y * FONT_SIZE + csl.margin);
                }
                break;
            default:
                drawchar(csl.x++ * CHAR_SIZE + csl. margin,
                         csl.y * FONT_SIZE + csl.margin,
                         str[i], color);
                break;
        }
    }
    return i;
}
