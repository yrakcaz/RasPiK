#include "../include/uart.h"
#include "../include/graphics.h"

//Shut up compiler!
#define DO_NOTHING_WITH(X) (void)(X)

//Kernel entry_point...
void k_start(uint32_t r0, uint32_t r1, uint32_t atags)
{
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);

    init_graphics();
    init_uart();

    uint32_t color = 0;

    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            putpixel(i, j, color++);

    //Stay alive...
    while (1) {
        write_uart((char *)".", 1);
        wait(100000000);
    }
}
