#include "../include/uart.h"
#include "../include/console.h"
#include "../include/mem.h"
#include "../include/interrupts.h"

//Shut up compiler!
#define DO_NOTHING_WITH(X) (void)(X)

//Time delay which a human can feel...
#define HUMAN_TIME 100000000

static void print_init(const char *module, int success)
{
    kwrite("[", 1, WHITE);
    if (success)
        kwrite("OK", 2, GREEN);
    else
        kwrite("KO", 2, RED);
    kwrite("]\t\t", 3, WHITE);
    kwrite("Module ", 7, WHITE);
    kwrite(module, strlen((char *)module), WHITE);
    kwrite(" initialization.\n", 17, WHITE);
    wait(HUMAN_TIME);
}

//Kernel entry_point...
void k_start(uint32_t r0, uint32_t r1, uint32_t atags)
{
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);

    //Initializations
    if (!init_graphics())
        return;
    init_console();
    print_init("graphics", 1);
    print_init("console", 1);
    init_uart();
    print_init("UART", 1);
    init_mem();
    print_init("memory", 1);
    init_interrupts();
    print_init("interrupts", 1);

    kwrite((char *)"\n\nKernel Booting ", 17, RED);

    //Stay alive...
    uint32_t k = get_timeval();
    uint32_t i = 0;
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
        if (get_timeval() != k)
            kwrite("Timer updated!\n", 15, WHITE);
    }
}
