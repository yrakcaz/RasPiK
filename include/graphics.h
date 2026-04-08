#ifndef GRAPHICS_H
# define GRAPHICS_H

# include <stdint.h>

# include "mailbox.h"
# include "utils.h"

# define FRAME_BUFFER    0x00002000
# define ARM_BUS_ADDR    0x40000000

# define SCREEN_WIDTH    800
# define SCREEN_HEIGHT   600
# define SCREEN_DEPTH    24

# define FONT_SIZE       16
# define CHAR_SIZE       8

# define BLACK      0x000000
# define WHITE      0xFFFFFF
# define RED        0xFF0000
# define GREEN      0x00FF00
# define BLUE       0x0000FF
# define YELLOW     0xFFFF00
# define BACKGROUND BLACK

typedef struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} s_color;

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

extern s_fb *framebuffer;

int init_graphics(void);
s_color hex_to_rgb(uint32_t color);
uint32_t rgb_to_hex(s_color color);
void put_pixel(uint32_t x, uint32_t y, uint32_t color);
uint32_t get_pixel(uint32_t x, uint32_t y);
void draw_char(uint32_t x, uint32_t y, char c, uint32_t color);
void clear_char(uint32_t x, uint32_t y);
void copy_char(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

#endif /* !GRAPHICS_H */
