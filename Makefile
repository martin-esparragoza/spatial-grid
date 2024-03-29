# Main called makefile. Calls other makefiles for building. Also has defined makefile variables.

CC = gcc
CFLAGS = -Wall -Ofast -march=native -mtune=native
CLINK = -lm
export

MAKEDIRS = lib/ src/

.PHONY: all clean $(MAKEDIRS)

all: $(MAKEDIRS)

clean: $(MAKEDIRS)

$(MAKEDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
