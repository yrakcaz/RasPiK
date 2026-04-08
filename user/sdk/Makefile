S_SRC = raspik.s
OBJ = $(S_SRC:.s=.o)
LIB = libraspik.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) -rcs $@ $^

%.o: %.s
	$(CC) -D__ASSEMBLY__ $(ARCHFLAGS) $(CONFIGFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(STDFLAGS) $(WARNFLAGS) $(ARCHFLAGS) $(CONFIGFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(LIB)
