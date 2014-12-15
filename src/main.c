#include "klog.h"
#include "mem.h"
#include "timers.h"
#include "interrupts.h"
#include "atags.h"
#include "syscall.h"
#include "process.h"
#include "scheduler.h"
#include "fs/vfs.h"

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
    init_scheduler();
    init_vfs();

    klog("\n\n", 2, WHITE);

    int fd = open("/tmp/lol", O_RDWR | O_CREAT);
    int fd1 = open("/tmp/toto", O_RDONLY | O_CREAT);
    int fd2 = open("/tmp/mdr", O_RDWR | O_CREAT);
    int fd3 = open("/dev/null", O_RDONLY | O_CREAT);
    int fd4 = open("/test/XD", O_APPEND);

    DO_NOTHING_WITH(fd);
    DO_NOTHING_WITH(fd1);
    DO_NOTHING_WITH(fd2);
    DO_NOTHING_WITH(fd3);
    DO_NOTHING_WITH(fd4);

    write(fd1, "testtest\n", 9);
    char *buff = kmalloc(10);
    seek(fd1, 0, SEEK_SET);
    read(fd1, buff, 9);
    klog(buff, strlen(buff), RED);
    kfree(buff);
    klog("\n\n", 2, WHITE);

    print_vfs();

    close(fd);
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    unmount("/tmp");

    print_vfs();

    //Continue...
    for (;;)
        draw_star();
}
