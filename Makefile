CROSS_PREFIX ?= /usr/local/cross/bin/arm-linux-
ASM_SRC := src/asm/boot.S src/asm/interrupts.S src/asm/utils.S
C_SRC := src/utils.c src/graphics.c src/uart.c src/console.c src/mem.c\
		 src/syscall.c src/interrupts.c src/atags.c src/vfs.c src/io.c src/main.c
OBJ := $(ASM_SRC:.S=.o)
OBJ += $(C_SRC:.c=.o)
LINK := src/rpi-link.ld
DFILES := $(shell find . -type f -name '*.d')

QEMU := 1

DEPENDFLAGS := -MD -MP
INCLUDES    := -I include
BASEFLAGS   := -fpic -pedantic -pedantic-errors -nostdlib
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
WARNFLAGS   := -Wall -Wextra -Wshadow
WARNFLAGS   += -Wredundant-decls -Winline
WARNFLAGS   += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS   += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS   += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS   += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS   += -Wmissing-include-dirs -Wno-multichar
WARNFLAGS   += -Wredundant-decls -Wshadow
WARNFLAGS   += -Wno-sign-compare -Wswitch -Wsystem-headers -Wundef
WARNFLAGS   += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS   += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS   += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
WARNFLAGS   += -Werror
SFLAGS      := $(INCLUDES) $(DEPENDFLAGS) -D__ASSEMBLY__
CFLAGS      := $(INCLUDES) $(DEPENDFLAGS) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS      += -I./include -std=gnu99
LDFLAGS     := -L/usr/local/cross/lib/gcc/arm-bcm2708hardfp-linux-gnueabi/4.7.1 -lgcc

include $(DFILES)
-include makefile.rules

all: kernel.img

kernel.elf: $(OBJ) $(LINK)
	$(CROSS_PREFIX)ld $(OBJ) -T$(LINK) -o $@ $(LDFLAGS)

kernel.img: kernel.elf
	$(CROSS_PREFIX)objcopy kernel.elf -O binary kernel.img

clean:
	rm -f $(OBJ) kernel.elf kernel.img

distclean: clean
	rm -f $(DFILES) makefile.rules

%.o: %.c
ifeq ($(QEMU), 1)
	$(CROSS_PREFIX)gcc -DQEMU $(CFLAGS) -c $< -o $@
else
	$(CROSS_PREFIX)gcc $(CFLAGS) -c $< -o $@
endif

%.o: %.S
	$(CROSS_PREFIX)gcc $(SFLAGS) -c $< -o $@

boot: kernel.elf
	qemu-system-arm -kernel kernel.elf -cpu arm1176 -m 256 -M raspi -serial stdio

debug: distclean
	./configure --with-debug
	$(MAKE) -C ./ _debug

_debug: kernel.elf
	qemu-system-arm -s -S -kernel kernel.elf -cpu arm1176 -m 256 -M raspi -serial stdio

install: kernel.img
	$(SHELL) scripts/script.sh

.PHONY: kernel.elf kernel.img configure
