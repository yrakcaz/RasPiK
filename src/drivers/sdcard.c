#include "drivers/sdcard.h"
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
    return emmc_issue_command((struct emmc_block_dev *)bd, cmd, args, HUMAN_TIME / 2);
}

int init_sdcard_driver(void)
{
    s_driver *driver = kmalloc(sizeof (s_driver));
    if (!driver)
        return 0;

    klog("[", 1, WHITE);
    klog("...", 3, RED);
    klog("]", 1, WHITE);

    driver->init = init_sdcard;
    driver->write = write_sdcard;
    driver->read = read_sdcard;
    driver->ioctl = ioctl_sdcard;

    int ret = insmod("/dev/sdcard", (void *)EMMC_BASE, driver);
    if (ret < 0)
    {
        klog("\b\b\b\bKO", 6, RED);
        klog("]\tSD card driver initialization failed.\n", 40, WHITE);
    }
    else
    {
        klog("\b\b\b\bOK", 6, GREEN);
        klog("]\tSD card driver initialized!\n", 30, WHITE);
    }

    return ret;
}
