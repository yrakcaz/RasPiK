#ifndef KLOG_H
# define KLOG_H

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
void init_klog(void);
int klog(const char *str, uint32_t size, uint32_t color);

#endif /* !KLOG_H */
