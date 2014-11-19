CROSS_PREFIX ?= /usr/local/cross/bin/arm-linux-
ASM_SRC := src/asm/boot.S
C_SRC := src/main.c
OBJ := $(ASM_SRC:.S=.o)
OBJ += $(C_SRC:.c=.o)
LINK := src/rpi-link.ld
DFILES := $(shell find . -type f -name '*.d')

DEPENDFLAGS := -MD -MP
INCLUDES    := -I include
BASEFLAGS   := -O2 -fpic -pedantic -pedantic-errors -nostdlib
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
WARNFLAGS   := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings
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
CFLAGS      += -std=c99

include $(DFILES)

all: kernel.img

kernel.elf: $(OBJ) $(LINK)
	$(CROSS_PREFIX)ld $(OBJ) -T$(LINK) -o $@

kernel.img: kernel.elf
	$(CROSS_PREFIX)objcopy kernel.elf -O binary kernel.img

clean:
	rm -f $(OBJ) kernel.elf kernel.img

distclean: clean
	rm -f $(DFILES)

%.o: %.c
	$(CROSS_PREFIX)gcc $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CROSS_PREFIX)gcc $(SFLAGS) -c $< -o $@

boot: kernel.elf
	qemu-system-arm -kernel kernel.elf -cpu arm1176 -m 256 -M versatilepb -serial stdio
