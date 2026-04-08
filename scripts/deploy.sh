#!/bin/sh

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <device>"
    exit 1
fi
DEV=$1

mkdir -p /media/boot
mount "$DEV" /media/boot
cp kernel.img /media/boot/kernel.img
umount "$DEV"
rmdir /media/boot
sync
