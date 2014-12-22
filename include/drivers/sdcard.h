#ifndef SDCARD_H
# define SDCARD_H

# include "drivers/imports/emmc.h"
# include "fs/devfs.h"

int init_sdcard_driver(void);

#endif /* !SDCARD_H */
