#ifndef KLOG_H
# define KLOG_H

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

void clear_klog(void);
void init_klog(void);
int klog_write(const char *str, uint32_t len, uint32_t color);
int klog_color(const char *str, uint32_t color);
int klog(const char *str);
void klog_ok(const char *msg);
void klog_ko(const char *msg);

#endif /* !KLOG_H */
