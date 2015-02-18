CROSS=/usr/local/cross/bin/arm-linux
CC=$(CROSS)-gcc
SRC=libraspik.c
OBJS=$(SRC:.c=.o)
TGT=libraspik.a

all: $(TGT)

$(TGT): $(OBJS)
	$(AR) -rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TGT)

distclean: clean
