# TODO

## Bootloader

- [x] Initialization
- [ ] Halt / Reboot / Sleep (WFI)
- [x] Fix wait
- [ ] Handle main boot parameters
- [ ] Multiboot support

## Kernel Logging

- [x] Basic klog implementation
- [ ] Optimize scrolling
- [ ] Redirect klogs to file at scheduler init
- [ ] printk / panic

## Memory Management

- [x] Dummy sbrk
- [x] kmalloc, kfree, kcalloc, krealloc
- [ ] Fix kfree
- [ ] Real memory management (segmentation, paging)

## Interrupts

- [x] IRQ / Timer
- [x] Exception vector table
- [ ] Handle all remaining interrupts and IRQs

## Syscalls

- [ ] Kernel land
- [ ] User land

## File Systems

- [x] Virtual file system with devices and mounting points
- [x] I/O (file management, devices)
- [x] Get file size
- [ ] FAT32
- [ ] Ext2
- [ ] Re-think device offsets
- [ ] ProcFS for process tracing
- [ ] Dup functions

## Drivers

- [x] UART
- [x] SD card (taken from rpi-boot)
- [ ] Mailbox property interface for framebuffer pixel order
- [ ] Recode UART driver properly
- [ ] Recode graphics and klog management
- [ ] HDMI / tty with buffer swapping
- [ ] LEDs
- [ ] GPIO
- [ ] USB
- [ ] Ethernet

## Processes & Scheduling

- [x] Process structure (with fd table and context)
- [x] Scheduler
- [x] Fork
- [x] ELF binary execution
- [x] Task context switching
- [ ] Priority queue and timing in scheduler
- [ ] Per-process fd table with STDIOs and file locking
- [ ] Environment variables

## Userland

- [ ] Switch to user mode
- [ ] libc basics (errno)

## General

- [ ] Make it work on real hardware
- [x] Reorganize boot and klog
