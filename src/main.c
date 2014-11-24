#include "../include/uart.h"
#include "../include/console.h"
#include "../include/mem.h"
#include "../include/interrupts.h"

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
    uint32_t state = 1;

    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);

    //Initializations
    if (!init_graphics())
        return;
    init_console();
    kwrite((char *)"Kernel Booting ...\n\n", 22, RED);
    print_init("graphics", 1);
    print_init("console", 1);
    init_uart();
    print_init("UART", 1);
    init_interrupts();
    print_init("interrupts", 1);

    kwrite("\n\n", 2, WHITE);

    //Stay alive...
    uint32_t i = gettick();
    while (state)
    {
        if (i != gettick())
        {
            i = gettick();
            char *time = itoa(i, 10);
            kwrite(time, strlen(time), WHITE);
            kwrite("\n", 1, WHITE);
        }
    }

    kwrite("*** SYSTEM HALTING ***\n", 23, RED);
}
