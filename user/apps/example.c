#include "raspik.h"

static void print(int fd, const char *s)
{
    uint32_t len = 0;
    while (s[len])
        len++;
    write(fd, s, len);
}

static void print_int(int fd, int n)
{
    if (n < 0) {
        write(fd, "-", 1);
        n = -n;
    }
    if (n == 0) {
        write(fd, "0", 1);
        return;
    }
    char buf[12];
    int i = 11;
    buf[i] = '\0';
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    print(fd, buf + i);
}

int main()
{
    int console = open("/dev/console", O_RDWR);
    if (console < 0)
        return 1;

    int pid = getpid();
    print(console, "getpid: ");
    print_int(console, pid);
    print(console, "\n");

    uint32_t t = gettime();
    print(console, "gettime: ");
    print_int(console, (int)t);
    print(console, " us\n");

    int bret = brk((void *)0);
    print(console, "brk: ");
    print_int(console, bret);
    print(console, "\n");

    int kret = kill(pid, 0);
    print(console, "kill: ");
    print_int(console, kret);
    print(console, "\n");

    int iret = ioctl(console, 0, 0);
    print(console, "ioctl: ");
    print_int(console, iret);
    print(console, "\n");

    print(console, "readdir /:");
    const char **root = readdir("/");
    if (root)
        for (int j = 0; root[j]; j++) {
            print(console, " ");
            print(console, root[j]);
        }
    print(console, "\n");

    print(console, "readdir /dev:");
    const char **devs = readdir("/dev");
    if (devs)
        for (int j = 0; devs[j]; j++) {
            print(console, " ");
            print(console, devs[j]);
        }
    print(console, "\n");

    print(console, "readdir /sdcard:");
    const char **sdc = readdir("/sdcard");
    if (sdc)
        for (int j = 0; sdc[j]; j++) {
            print(console, " ");
            print(console, sdc[j]);
        }
    print(console, "\n");

    int sfd = open("/sdcard/example.bin", O_RDONLY);
    if (sfd >= 0) {
        s_stat sb;
        int sret = stat(sfd, &sb);
        print(console, "stat example.bin: ");
        if (sret == 0) {
            print_int(console, (int)sb.st_size);
            print(console, " bytes");
        } else {
            print(console, "err ");
            print_int(console, sret);
        }
        print(console, "\n");

        print(console, "seek SEEK_SET: ");
        print_int(console, seek(sfd, 0, SEEK_SET));
        print(console, "\n");

        char fbuf[16];
        print(console, "read: ");
        print_int(console, read(sfd, fbuf, sizeof(fbuf)));
        print(console, " bytes\n");

        print(console, "seek SEEK_CUR: ");
        print_int(console, seek(sfd, 4, SEEK_CUR));
        print(console, "\n");

        print(console, "seek SEEK_END: ");
        print_int(console, seek(sfd, 0, SEEK_END));
        print(console, "\n");

        close(sfd);
    }

    print(console, "mount /userfs: ");
    print_int(console, mount("", "/userfs", VFILES));
    print(console, "\n");

    int fd = open("/userfs/hello", O_CREAT | O_RDWR);
    if (fd >= 0) {
        print(console, "write vffs: ");
        print_int(console, write(fd, "world", 5));
        print(console, " bytes\n");

        seek(fd, 0, SEEK_SET);

        char vbuf[8];
        print(console, "read vffs: ");
        print_int(console, read(fd, vbuf, sizeof(vbuf)));
        print(console, " bytes\n");

        s_stat vsb;
        stat(fd, &vsb);
        print(console, "stat vffs: ");
        print_int(console, (int)vsb.st_size);
        print(console, " bytes\n");

        close(fd);
    }

    int afd = open("/userfs/hello", O_APPEND | O_RDWR);
    if (afd >= 0) {
        write(afd, "!", 1);
        close(afd);
    }

    print(console, "readdir /userfs:");
    const char **uf = readdir("/userfs");
    if (uf)
        for (int j = 0; uf[j]; j++) {
            print(console, " ");
            print(console, uf[j]);
        }
    print(console, "\n");

    print(console, "chmod: ");
    print_int(console, chmod("/userfs/hello", O_RDONLY));
    print(console, "\n");

    print(console, "remove: ");
    print_int(console, remove("/userfs/hello"));
    print(console, "\n");

    print(console, "unmount /userfs: ");
    print_int(console, unmount("/userfs"));
    print(console, "\n");

    close(console);
    return 0;
}
