# This makefile is supposed to compile all object-worthy files.

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all clean

all: $(OBJS)

clean:
	rm -rf *.o

%.o: %.c ../include/%.h
	$(CC) -c $< $(CFLAGS) -o $@
