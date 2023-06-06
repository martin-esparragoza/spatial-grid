# Compiles and links all executable files.

# Previously compiled objects from /lib
OBJS = $(wildcard ../lib/*.o) # Should be dynamic
EXECS = test_vector.o

.PHONY: all clean

all: $(EXECS)

clean:
	rm -rf *.o

$(OBJS):
	# Remove circular dependency

%.o: %.c $(OBJS)
	$(CC) $< $(CFLAGS) -o $@ $(CLINK) $(OBJS)