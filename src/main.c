#include "../include/uart.h"
#include "../include/graphics.h"

//Shut up compiler!
#define DO_NOTHING_WITH(X) (void)(X)

//Time delay which a human can feel...
#define HUMAN_TIME 100000000

//Kernel entry_point...
void k_start(uint32_t r0, uint32_t r1, uint32_t atags)
{
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);

    //Initializations
    init_graphics();
    init_uart();

    write_uart((char *)"Kernel Booting", 14);

    //Stay alive...
    while (1) {
        write_uart((char *)".", 1);
        wait(HUMAN_TIME);
    }
}
