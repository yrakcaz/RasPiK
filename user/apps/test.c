#include "../sdk/libraspik.h"

int main()
{
    int fd = open("/dev/uart", O_RDWR);
    write(fd, "zbrah\n", 6);
}
