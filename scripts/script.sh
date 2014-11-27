#! env sh

sudo mkdir -p /media/boot
sudo mount /dev/sde1 /media/boot
sudo rm /media/boot/kernel.img
sudo cp kernel.img /media/boot
sudo umount /dev/sde1
sudo rmdir /media/boot
sync
