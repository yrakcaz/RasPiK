CROSS=/usr/local/cross/bin/arm-linux
CC=$(CROSS)-gcc
C_SRC=libraspik.c
S_SRC=libraspik-asm.S
OBJS=$(S_SRC:.S=.o)
OBJS+=$(C_SRC:.c=.o)
TGT=libraspik.a

all: $(TGT)

$(TGT): $(OBJS)
	$(AR) -rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CROSS)-gcc -D__ASSEMBLY__ -c $< -o $@

clean:
	rm -f $(OBJS) $(TGT)

distclean: clean
