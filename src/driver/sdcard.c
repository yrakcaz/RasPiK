#include "driver/sdcard.h"
#include "fs/vfs.h"

static struct block_device *bd = NULL;

static int init_sdcard(s_device *dev)
{
    DO_NOTHING_WITH(dev);
    return emmc_card_init(&bd);
}

static int read_sdcard(s_device *dev, uint32_t *offset, void *buff, uint32_t len)
{
    DO_NOTHING_WITH(dev);
    return emmc_read(bd, buff, len, *offset);
}

static int write_sdcard(s_device *dev, uint32_t *offset, const void *buff, uint32_t len)
{
    DO_NOTHING_WITH(dev);
    return emmc_write(bd, (void *)buff, len, *offset);
}

static int ioctl_sdcard(s_device *dev, int cmd, int args)
{
    DO_NOTHING_WITH(dev);
    return emmc_issue_command((struct emmc_block_dev *)bd, cmd, args, USEC_PER_SEC / 2);
}

int init_sdcard_driver(void)
{
    s_driver *driver = kmalloc(sizeof (s_driver));
    if (!driver)
        return 0;

    driver->init = init_sdcard;
    driver->write = write_sdcard;
    driver->read = read_sdcard;
    driver->ioctl = ioctl_sdcard;

    int ret = insmod("/dev/sdcard", (void *)EMMC_BASE, driver);
    if (ret < 0)
        klog_ko("SD card driver initialization failed");
    else
        klog_ok("SD card driver initialized");

    return ret;
}
