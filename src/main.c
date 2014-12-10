#include "klog.h"
#include "mem.h"
#include "timers.h"
#include "interrupts.h"
#include "atags.h"
#include "syscall.h"
#include "vfs.h"
#include "io.h"
#include "driver.h"
#include "process.h"
#include "scheduler.h"

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

void draw_star(void)
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

    init_process();
    print_init("process", 1);

    print_init("scheduler", init_scheduler());

    klog("\n\n", 2, WHITE);

#ifdef QEMU
    DO_NOTHING_WITH(atags);
#else
    print_atags(atags);
#endif

    for (int i = 0; i < 10; i++)
        draw_star();
    clear_klogs();

    add_process("process1", (uint32_t)&process1);
    add_process("process2", (uint32_t)&process2);

    for (;;);
}
