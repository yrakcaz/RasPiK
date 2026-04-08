[![CI](https://github.com/yrakcaz/RasPiK/actions/workflows/ci.yml/badge.svg)](https://github.com/yrakcaz/RasPiK/actions/workflows/ci.yml)
[![MIT License](https://img.shields.io/github/license/yrakcaz/RasPiK?color=blue)](./LICENSE)

# RasPiK

A lightweight bare-metal kernel for the Raspberry Pi (ARM1176JZF-S), written in C. Built as a learning project to explore ARM architecture, OS fundamentals, and embedded systems development. Designed to run on QEMU during development, with real hardware as the end target.

## Sample boot output

```
[OK] Klogs initialized
[OK] Timers initialized
[OK] Interrupt vector initialized
[OK] Syscall table initialized
[OK] Processes initialized
[OK] Virtual File System initialized
[OK] Console driver initialized
[OK] UART driver initialized
[OK] SD card driver initialized
[OK] Scheduler initialized
[OK] sdcard mounted
 ____           ____  _ _  __
|  _ \ __ _ ___|  _ \(_) |/ /
| |_) / _` / __| |_) | | ' /
|  _ < (_| \__ \  __/| | . \
|_| \_\__,_|___/_|   |_|_|\_\

/tmp (vffs)
/dev (devfs)
        console uart sdcard
/sdcard (fat32)
        EXAMPLE.BIN
```

## Features

- ARM assembly bootloader with privilege mode and stack initialization
- Kernel logging with color output and serial output
- Memory management: `kmalloc`, `kfree`, `kcalloc`, `krealloc`
- Interrupt and IRQ handling with an ARM exception vector table
- Timer-based preemptive scheduler
- Virtual File System with support for devices, virtual files, and FAT32
- Process management: fork, exec (ELF binaries), context switching
- Syscall interface bridging user and kernel space
- UART and SD card drivers
- QEMU support for development and testing

## Requirements

- `arm-none-eabi-gcc` and `arm-none-eabi-binutils` cross-compiler toolchain
- `qemu-system-arm` for running under QEMU
- `dosfstools` for SD card image creation

On Debian/Ubuntu:

```
apt install gcc-arm-none-eabi binutils-arm-none-eabi qemu-system-arm dosfstools mtools
```

## Build

For QEMU:

```
./configure --with-qemu && make
```

For real hardware:

```
./configure && make
```

To enable debug symbols, add `--with-debug` to either configure invocation.

## Run (QEMU)

```
scripts/qemu.sh
```

Options:

```
scripts/qemu.sh --vnc          # VNC output on port 5901
scripts/qemu.sh --no-graphics  # no display output
scripts/qemu.sh --debug        # start GDB server on port 1234
```

## Install on hardware

Copy the kernel to a physical SD card:

```
scripts/deploy.sh <device>
```

## Project Structure

```
src/            kernel C source files
src/asm/        ARM assembly (boot, interrupt vector, utils)
src/driver/     UART and SD card drivers
src/fs/         VFS, FAT32, devfs, vffs, partitions
include/        kernel headers
include/driver/ driver headers
include/fs/     filesystem headers
import/         third-party source imported verbatim (see import/README.md)
user/apps/      user-space example application
user/sdk/       user-space SDK (syscall wrappers and headers)
scripts/        build and deployment helper scripts
```
