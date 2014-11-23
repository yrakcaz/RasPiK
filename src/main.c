#include "../include/uart.h"
#include "../include/console.h"
#include "../include/mem.h"

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
    init_mem();

    kwrite((char *)"Kernel Booting ", 15, RED);

    //Stay alive...
    int i = 0;
    while (1)
    {
        if (i == 3)
        {
            i = -1;
            kwrite("\b\b\b", 3, WHITE);
        }
        else
            kwrite(".", 1, BLUE);
        wait(HUMAN_TIME);
        i++;
    }
}
