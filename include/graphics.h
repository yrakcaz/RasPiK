#ifndef GRAPHICS_H
# define GRAPHICS_H

# include <stdint.h>
# include "utils.h"
# include "mailbox.h"

/* Usefull defines for graphic driver. */
# define FRAME_BUFFER    0x00002000
# define ARM_BUS_ADDR    0x40000000

# define SCREEN_WIDTH    800
# define SCREEN_HEIGHT   600
# define SCREEN_DEPTH    24

# define FONT_SIZE       16
# define CHAR_SIZE       8

/* Colors */
# define BLACK  0x000000
# define WHITE  0xFFFFFF
# define RED    0xFF0000
# define GREEN  0x00FF00
# define BLUE   0x0000FF
# define YELLOW 0xFFFF00
# define BCKG   BLACK

/* RGB structure */
typedef struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} s_color;

/* Structure which represents the rpi framebuffer */
typedef struct framebuffer
{
    uint32_t width;
    uint32_t height;
    uint32_t v_width;
    uint32_t v_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x_off;
    uint32_t y_off;
    uint32_t ptr;
    uint32_t size;
} s_fb;

/* Then its instantiation! */
s_fb *fb;

/* External functions... */
int init_graphics(void);
s_color hex_to_rgb(uint32_t color);
uint32_t rgb_to_hex(s_color color);
void putpixel(uint32_t x, uint32_t y, uint32_t color);
uint32_t getpixel(uint32_t x, uint32_t y);
void drawchar(uint32_t x, uint32_t y, char c, uint32_t color);
void deletechar(uint32_t x, uint32_t y);
void replychar(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

#endif /* !GRAPHICS_H */
