#include "klog.h"
#include "mem.h"
#include "timers.h"
#include "interrupts.h"
#include "atags.h"
#include "syscall.h"
#include "process.h"
#include "scheduler.h"
#include "fs/vfs.h"
#include "drivers/uart.h"
#include "drivers/sdcard.h"

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
    //We'll treat these after if needed...
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags); // Use print_atags to display it...

    //Initializations
    init_graphics();
    init_klog();
    init_timers();
    init_interrupts();
    init_syscalls();
    init_process();
    init_vfs();
    init_uart_driver();
    init_sdcard_driver();
    mount_devices();
    init_scheduler();

    klog("\n\n", 2, WHITE);

    print_vfs();

    klog("\n\n", 2, WHITE);

    for (;;)
        draw_star();
}
