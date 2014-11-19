#include "../include/uart.h"

//Shut up compiler!
#define DO_NOTHING_WITH(X) (void)(X)

//Kernel entry_point...
void k_start(uint32_t r0, uint32_t r1, uint32_t atags)
{
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);
   
    init_uart();

    putstring_uart("******************************\n");
    putstring_uart("       KENEL LOADING...\n");
    putstring_uart("******************************\n");

    //Stay alive...
    while (1) {}
}
