#include "atags.h"
#include "driver/sdcard.h"
#include "driver/uart.h"
#include "fs/vfs.h"
#include "interrupts.h"
#include "klog.h"
#include "mem.h"
#include "process.h"
#include "scheduler.h"
#include "syscall.h"
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
        klogc(lines[i], YELLOW);
    klog("\n");
}

static void spinner(void)
{
    static int i = 0;
    switch (i)
    {
      case 0:
       klogc("-", RED);
       break;
      case 1:
       klogc("\\", GREEN);
       break;
      case 2:
       klogc("|", BLUE);
       break;
      case 3:
       klogc("/", YELLOW);
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
    init_uart_driver();
    init_sdcard_driver();
    mount_devices();
    init_scheduler();

    print_banner();
    print_vfs();
    klog("\n");

    for (;;)
        spinner();
}
