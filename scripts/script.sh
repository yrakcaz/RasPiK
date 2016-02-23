#!/bin/sh

sudo mkdir -p /media/boot
sudo mount /dev/mmcblk0p1 /media/boot
sudo rm /media/boot/kernel.img
sudo cp kernel.img /media/boot
sudo umount /dev/mmcblk0p1
sudo rmdir /media/boot
sync
