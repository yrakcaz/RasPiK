#include "atags.h"
#include "driver/console.h"
#include "driver/sdcard.h"
#include "driver/uart.h"
#include "fs/vfs.h"
#include "interrupts.h"
#include "klog.h"
#include "mem.h"
#include "process.h"
#include "scheduler.h"
#include "syscalls.h"
#include "timers.h"

static void print_banner(void)
{
    const char *lines[] = {
        " ____           ____  _ _  __\n",
        "|  _ \\ __ _ ___|  _ \\(_) |/ /\n",
        "| |_) / _` / __| |_) | | ' /\n",
        "|  _ < (_| \\__ \\  __/| | . \\\n",
        "|_| \\_\\__,_|___/_|   |_|_|\\_\\\n",
        NULL
    };
    for (int i = 0; lines[i]; i++)
        klog_color(lines[i], YELLOW);
    klog("\n");
}

static void spinner(void)
{
    static int i = 0;
    switch (i)
    {
      case 0:
       klog_color("-", RED);
       break;
      case 1:
       klog_color("\\", GREEN);
       break;
      case 2:
       klog_color("|", BLUE);
       break;
      case 3:
       klog_color("/", YELLOW);
       i = -1;
       break;
      default:
       break;
    }
    usleep(USEC_PER_SEC / 3);
    klog("\b");
    i++;
}

void k_start(uint32_t r0, uint32_t r1, s_aheader *atags)
{
    DO_NOTHING_WITH(r0);
    DO_NOTHING_WITH(r1);
    DO_NOTHING_WITH(atags);

    init_graphics();
    init_klog();
    init_timers();
    init_interrupts();
    init_syscalls();
    init_process();
    init_vfs();
    init_console_driver();
    init_uart_driver();
    init_sdcard_driver();
    init_scheduler();

    if (mount("/dev/sdcard", "sdcard", FAT32) < 0)
        klog_ko("Failed to mount sdcard");
    else
        klog_ok("sdcard mounted");

    print_banner();
    print_vfs();
    klog("\n");

    for (;;)
        spinner();
}
