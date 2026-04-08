SRC = example.c
BIN = $(SRC:.c=.bin)
OBJ = $(SRC:.c=.o)
LDSCRIPT = ../sdk/raspik.ld
LDFLAGS := $(shell $(CC) -print-libgcc-file-name) -L../sdk -lraspik

all: $(BIN)

%.bin: %.o $(LDSCRIPT)
	$(LD) $< -T$(LDSCRIPT) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -I../sdk $(STDFLAGS) $(WARNFLAGS) $(ARCHFLAGS) $(CONFIGFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.bin
