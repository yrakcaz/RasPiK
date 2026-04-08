#!/bin/sh

set -e

usage() {
    echo "Usage: scripts/qemu.sh [options]"
    echo "  --vnc          use VNC display (port 5901)"
    echo "  --no-graphics  disable display output"
    echo "  --debug        start GDB server on port 1234 (-s -S)"
    echo "  --help         show this help"
}

vnc=0
no_graphics=0
debug=0

for arg in "$@"; do
    case "$arg" in
        --vnc)         vnc=1 ;;
        --no-graphics) no_graphics=1 ;;
        --debug)       debug=1 ;;
        --help)        usage; exit 0 ;;
        *)             echo "Unknown option: $arg"; usage; exit 1 ;;
    esac
done

if [ "$vnc" = 1 ] && [ "$no_graphics" = 1 ]; then
    echo "Error: --vnc and --no-graphics are mutually exclusive"
    exit 1
fi

"$(dirname "$0")/mksdcard.sh"

if [ "$vnc" = 1 ]; then
    DISPLAY_ARG="-vnc :1"
elif [ "$no_graphics" = 1 ]; then
    DISPLAY_ARG="-display none"
else
    DISPLAY_ARG=""
fi

DEBUG_ARG=""
if [ "$debug" = 1 ]; then
    DEBUG_ARG="-s -S"
fi

qemu-system-arm \
    -kernel kernel.elf \
    -cpu arm1176 \
    -m 512 \
    -M raspi1ap \
    -serial stdio \
    $DISPLAY_ARG \
    $DEBUG_ARG \
    -drive format=raw,file=sdcard.img,if=sd
