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

    //TESTS/////////////////

    klog("\n\n", 2, WHITE);

    print_vfs();

    klog("\n\n", 2, WHITE);

    int fd1 = open("/sdcard/lol.mp3", O_RDONLY);
    if (fd1 < 0)
    {
        klog("fd1 failed!\n", 12, RED);
        goto end;
    }
    else
        klog("fd1 good!\n", 10, GREEN);
    int fd2 = open("/sdcard/test.false", O_RDONLY);
    if (fd2 < 0)
        klog("fd2 good!\n", 10, GREEN);
    else
        klog("fd2 failed!\n", 12, RED);

    if (ioctl(fd1, 0, 0) < 0)
        klog("ioctl good!\n", 12, GREEN);
    else
        klog("ioctl failed!\n", 14, RED);
    if (seek(fd1, 0, SEEK_SET) < 0)
        klog("seek failed!\n", 13, RED);
    else
        klog("seek good!\n", 11, GREEN);

    s_stat sb;
    if (stat(fd1, &sb) < 0)
    {
        klog("stat failed!\n", 13, RED);
        goto close;
    }
    klog("Size : ", 7, YELLOW);
    klog(itoa(sb.st_size, 10), strlen(itoa(sb.st_size, 10)), YELLOW);
    klog("\n", 1, YELLOW);

    char *readtest = kmalloc(sb.st_size + 1);
    if (!readtest)
    {
        klog("mem fail!\n", 10, RED);
        goto close;
    }
    if (read(fd1, readtest, sb.st_size) != sb.st_size)
        klog("read failed!\n", 13, RED);
    else
    {
        readtest[sb.st_size] = 0;
        klog(readtest, sb.st_size, GREEN);
        klog("\n", 1, WHITE);
    }

close:
    if (close(fd1) < 0)
        klog("close failed!\n", 14, RED);
    else
        klog("close good!\n", 12, GREEN);

end:
    klog("\n\n", 2, WHITE);

    //TESTS/////////////////

    for (;;)
        draw_star();
}
