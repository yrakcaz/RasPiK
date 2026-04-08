SRC := test.c
OBJ := $(SRC:.c=.o)
LINK := apps.ld
LDFLAGS := $(shell $(CC) -print-libgcc-file-name)

all: test.bin

test.bin: $(OBJ) $(LINK)
	$(LD) $(OBJ) -T$(LINK) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.bin
