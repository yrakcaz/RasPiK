#include "uart.h"
#include "console.h"
#include "mem.h"
#include "interrupts.h"
#include "atags.h"
#include "sd.h"

//Time delay which a human can feel...
# define HUMAN_TIME 1000000

static void print_init(const char *module, int success)
{
    write_console("[", 1, WHITE);
    if (success)
        write_console("OK", 2, GREEN);
    else
        write_console("KO", 2, RED);
    write_console("]\t\t", 3, WHITE);
    write_console("Module ", 7, WHITE);
    write_console(module, strlen((char *)module), WHITE);
    write_console(" initialization.\n", 17, WHITE);
    wait(HUMAN_TIME);
}

//Kernel entry_point...
void k_start(uint32_t r0, uint32_t r1, s_aheader *atags)
{
    uint32_t state = 1;

    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);

    //Initializations
    if (!init_graphics())
        return;
    init_console();
    write_console((char *)"Kernel Booting ...\n\n", 22, RED);
    print_init("graphics", 1);
    print_init("console", 1);
    init_uart();
    print_init("UART", 1);
#ifdef QEMU //Debug it before use it on real device.
    init_interrupts();
    print_init("interrupts", 1);
#else
    print_init("interrupts", 0);
#endif
    print_init("sdcard", init_sd(0));

    write_console("\n\n", 2, WHITE);

#ifdef QEMU
    DO_NOTHING_WITH(atags);
#else
    print_atags(atags);
#endif

    //Stay alive...
    uint32_t i = gettick();
    while (state)
    {
        if (i != gettick())
        {
            i = gettick();
            char *time = itoa(i, 10);
            write_console(time, strlen(time), WHITE);
            write_console("\n", 1, WHITE);
        }
    }
}
