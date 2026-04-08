#!/bin/sh

set -e

SFDISK=$(command -v sfdisk || command -v /usr/sbin/sfdisk) || { echo "sfdisk not found"; exit 1; }
MKFSFAT=$(command -v mkfs.fat || command -v /usr/sbin/mkfs.fat) || { echo "mkfs.fat not found"; exit 1; }

dd if=/dev/zero of=sdcard.img bs=1M count=32
echo '2048,,0B;' | "$SFDISK" sdcard.img
"$MKFSFAT" -F 32 --offset 2048 sdcard.img
mcopy -oi sdcard.img@@1048576 user/apps/test.bin ::
