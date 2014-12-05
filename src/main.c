#include "klog.h"
#include "mem.h"
#include "timers.h"
#include "interrupts.h"
#include "atags.h"
#include "syscall.h"
#include "vfs.h"
#include "io.h"
#include "driver.h"

//Time delay which a human can feel...
# define HUMAN_TIME 1000000

static void print_init(const char *module, int success)
{
    klog("[", 1, WHITE);
    if (success)
        klog("OK", 2, GREEN);
    else
        klog("KO", 2, RED);
    klog("]\t\t", 3, WHITE);
    klog("Module ", 7, WHITE);
    klog(module, strlen(module), WHITE);
    klog(" initialization.\n", 17, WHITE);
    wait(HUMAN_TIME);
}

static void draw_star(void)
{
    static int i = 0;
    switch (i)
    {
        case 0:
            klog("-", 1, RED);
            break;
        case 1:
            klog("\\", 1, GREEN);
            break;
        case 2:
            klog("|", 1, BLUE);
            break;
        case 3:
            klog("/", 1, YELLOW);
            i = -1;
            break;
        default:
            break;
    }
    wait(HUMAN_TIME / 3);
    klog("\b", 1, WHITE);
    i++;
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
    init_klog();
    init_timers();
    klog((char *)"Kernel Booting ...\n\n", 22, RED);
    print_init("timers", 1);
    print_init("graphics", 1);
    print_init("klog", 1);
#ifdef QEMU //Debug it before use it on real device.
    init_interrupts();
    print_init("interrupts", 1);
#else
    print_init("interrupts", 0);
#endif
    init_syscalls();
    print_init("syscalls", 1);

    print_init("vfs", init_vfs());
    print_init("IO", init_io());
    print_init("drivers", init_drivers());

    klog("\n\n", 2, WHITE);

#ifdef QEMU
    DO_NOTHING_WITH(atags);
#else
    print_atags(atags);
#endif

    //Stay alive...
    uint64_t t = get_time();
    int i = 0;
    while (state)
    {
        uint64_t u = get_time();
        if (u >= t + HUMAN_TIME)
        {
            t = u;
            klog(itoa(i, 10), strlen(itoa(i, 10)), RED);
            klog("\n", 1, RED);
            i++;
        }
    }
    draw_star();
}
