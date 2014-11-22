#include "../include/uart.h"
#include "../include/console.h"

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
    init_uart();
    if (!init_graphics())
        write_uart((char *)"Error while loading graphics!\n", 30);
    init_console();

    kwrite((char *)"Kernel Booting ", 14, BLUE);

    //Stay alive...
    while (1)
    {
        kwrite(".", 1, BLUE);
        wait(HUMAN_TIME / 100);
    }
}
