CROSS_PREFIX ?= /usr/local/cross/bin/arm-linux-
SRC := test.c
OBJ := $(SRC:.c=.o)
LINK := apps.ld
LDFLAGS     := -L/usr/local/cross/lib/gcc/arm-bcm2708hardfp-linux-gnueabi/4.7.1 -lgcc -L../sdk -lraspik

all: test.bin

test.bin: $(OBJ) $(LINK)
	$(CROSS_PREFIX)ld $(OBJ) -T$(LINK) -o $@ $(LDFLAGS)

%.o: %.c
	$(CROSS_PREFIX)gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.bin

distclean: clean
