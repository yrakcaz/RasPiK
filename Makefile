CROSS = arm-none-eabi
export CC = $(CROSS)-gcc
export LD = $(CROSS)-ld
export AR = $(CROSS)-ar
export OBJCOPY = $(CROSS)-objcopy

S_SRC = src/asm/boot.s src/asm/interrupts.s src/asm/utils.s
C_SRC = src/atags.c src/elf.c src/graphics.c src/interrupts.c \
        src/klog.c src/mailbox.c src/main.c src/mem.c         \
        src/process.c src/scheduler.c src/syscalls.c          \
        src/timers.c src/utils.c
C_SRC += src/driver/console.c src/driver/sdcard.c src/driver/uart.c
C_SRC += src/fs/devfs.c src/fs/fat32.c src/fs/mbr.c src/fs/vffs.c src/fs/vfs.c
C_SRC += import/src/emmc.c

OBJ = $(S_SRC:.s=.o)
OBJ += $(C_SRC:.c=.o)
LDSCRIPT = src/raspik-kernel.ld
DFILES := $(shell find . -type f -name '*.d')

CONFIGFLAGS =
DEPENDFLAGS = -MD -MP
STDFLAGS = -std=gnu99
ARCHFLAGS = -mcpu=arm1176jzf-s
KERNELFLAGS = -fpic -nostdlib
KERNELFLAGS += -ffreestanding -fomit-frame-pointer
WARNFLAGS = -Wall -Wdisabled-optimization -Werror -Wextra              \
            -Wfloat-equal -Wformat=2 -Winit-self -Winline              \
            -Winvalid-pch -Wmissing-format-attribute                   \
            -Wmissing-include-dirs -Wno-attributes                     \
            -Wno-deprecated-declarations -Wno-div-by-zero              \
            -Wno-endif-labels -Wno-format-extra-args -Wno-multichar    \
            -Wno-pragmas -Wno-sign-compare                             \
            -Wno-unused-but-set-parameter -Wno-unused-but-set-variable \
            -Wno-unused-result -Wpointer-arith -Wredundant-decls       \
            -Wshadow -Wswitch -Wsystem-headers -Wundef -Wwrite-strings
ASFLAGS = $(DEPENDFLAGS) -D__ASSEMBLY__ $(ARCHFLAGS) $(CONFIGFLAGS)
CFLAGS = -Iinclude/ -Iimport/include/ $(DEPENDFLAGS) $(STDFLAGS) $(KERNELFLAGS) \
         $(WARNFLAGS) $(ARCHFLAGS) $(CONFIGFLAGS)
LDFLAGS := $(shell $(CC) -print-libgcc-file-name)

export ARCHFLAGS
export CONFIGFLAGS
export STDFLAGS
export WARNFLAGS

all: kernel apps

-include makefile.rules
-include $(DFILES)

kernel: kernel.img

kernel.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img

kernel.elf: $(OBJ) $(LDSCRIPT)
	$(LD) $(OBJ) -T$(LDSCRIPT) -o $@ $(LDFLAGS)

%.o: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

apps: sdk
	$(MAKE) -C user/apps/

sdk:
	$(MAKE) -C user/sdk/

distclean: clean
	$(RM) makefile.rules

clean:
	$(RM) $(OBJ) $(DFILES) kernel.elf kernel.img
	$(MAKE) -C user/sdk/ clean
	$(MAKE) -C user/apps/ clean

.PHONY: all kernel apps sdk clean distclean
