CROSS := arm-none-eabi
export CC := $(CROSS)-gcc
export LD := $(CROSS)-ld
export OBJCOPY := $(CROSS)-objcopy

ASM_SRC := src/asm/boot.s src/asm/interrupts.s src/asm/utils.s
C_SRC := src/utils.c src/mailbox.c src/graphics.c src/klog.c src/atags.c src/mem.c         \
		 src/syscall.c src/timers.c src/interrupts.c src/process.c src/elf.c           \
		 src/scheduler.c src/main.c
C_SRC += src/fs/parts.c src/fs/vffs.c src/fs/devfs.c src/fs/fat32.c src/fs/vfs.c
C_SRC += src/driver/uart.c src/driver/sdcard.c
C_SRC += import/src/emmc.c

OBJ := $(ASM_SRC:.s=.o)
OBJ += $(C_SRC:.c=.o)
LINK := src/rpi-link.ld
DFILES := $(shell find . -type f -name '*.d')

DEPENDFLAGS := -MD -MP
INCLUDES := -Iinclude/ -Iimport/include/
BASEFLAGS := -fpic -nostdlib -std=gnu99
BASEFLAGS += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
WARNFLAGS := -Wall -Wextra -Wshadow
WARNFLAGS += -Wredundant-decls -Winline
WARNFLAGS += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS += -Wmissing-include-dirs -Wno-multichar
WARNFLAGS += -Wno-sign-compare -Wswitch -Wsystem-headers -Wundef
WARNFLAGS += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
WARNFLAGS += -Werror
SFLAGS := $(INCLUDES) $(DEPENDFLAGS) -D__ASSEMBLY__ -mcpu=arm1176jzf-s
CFLAGS := $(INCLUDES) $(DEPENDFLAGS) $(BASEFLAGS) $(WARNFLAGS)
LDFLAGS := $(shell $(CC) -print-libgcc-file-name)

all: kernel user

-include makefile.rules
-include $(DFILES)

kernel: kernel.img

user:
	$(MAKE) -C user/apps/

kernel.elf: $(OBJ) $(LINK)
	$(LD) $(OBJ) -T$(LINK) -o $@ $(LDFLAGS)

kernel.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img

clean:
	$(RM) $(OBJ) $(DFILES) kernel.elf kernel.img
	$(MAKE) clean -C user/apps/

distclean: clean
	$(RM) makefile.rules

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(SFLAGS) -c $< -o $@

.PHONY: all kernel user clean distclean
