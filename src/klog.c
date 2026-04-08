#include "driver/uart.h"
#include "klog.h"

static s_console console;

void clear_klog(void)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            put_pixel(i, j, BLACK);
    console.x = 0;
    console.y = 0;
}

void init_klog(void)
{
    console.x = 0;
    console.y = 0;
    console.margin = MARGIN;
    clear_klog();
    klog_ok("Klogs initialized");
}

static void scroll(void)
{
    for (int i = 0; i < NBCOL; i++)
        clear_char(console.x * CHAR_SIZE + console.margin, console.margin);
    for (int i = 0; i < NBCOL; i++)
    {
        for (int j = 1; j < NBLIN; j++)
        {
            copy_char(i * CHAR_SIZE + console.margin,
                      j * FONT_SIZE + console.margin,
                      i * CHAR_SIZE + console.margin,
                      (j - 1) * FONT_SIZE + console.margin);
            clear_char(i * CHAR_SIZE + console.margin,
                       j * FONT_SIZE + console.margin);
        }
    }
}

int klog_write(const char *str, uint32_t len, uint32_t color)
{
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        if (str[i] == '\n')
            putchar_uart('\r');
        putchar_uart(str[i]);

        if (console.x >= NBCOL)
        {
            console.x = 0;
            if (console.y == NBLIN - 1)
                scroll();
            else
                console.y++;
        }

        switch (str[i])
        {
          case '\n':
           console.x = 0;
           if (console.y == NBLIN - 1)
               scroll();
           else
               console.y++;
           break;
          case '\t':
           if (console.x + 4 < NBCOL)
               console.x += 4;
           break;
          case '\b':
           if (console.x > 0)
           {
               console.x--;
               clear_char(console.x * CHAR_SIZE + console.margin,
                          console.y * FONT_SIZE + console.margin);
           }
           break;
          default:
           draw_char(console.x++ * CHAR_SIZE + console.margin,
                     console.y * FONT_SIZE + console.margin,
                     str[i], color);
           break;
        }
    }
    return i;
}

int klog_color(const char *str, uint32_t color)
{
    return klog_write(str, strlen(str), color);
}

int klog(const char *str)
{
    return klog_write(str, strlen(str), WHITE);
}

static void klog_tag(const char *tag, uint32_t color, const char *msg)
{
    klog("[");
    klog_color(tag, color);
    klog("] ");
    klog(msg);
    klog("\n");
}

void klog_ok(const char *msg)
{
    klog_tag("OK", GREEN, msg);
}

void klog_ko(const char *msg)
{
    klog_tag("KO", RED, msg);
}
