bin		:= usha256

src		:=
src		+= main.c

obj		:= ${src:.c=.o}

dep		:= $(wildcard *.d)

deb		:= 0

CC		:= clang

CPPFLAGS	:=
CPPFLAGS	+= -MMD

CFLAGS		:=

CFLAGS		+= -fno-PIE
CFLAGS		+= -pedantic
CFLAGS		+= -pedantic-errors
CFLAGS		+= -Wall
CFLAGS		+= -Werror
CFLAGS		+= -Wextra
CFLAGS		+= -Wundef

CFLAGS		+= -I .
CFLAGS		+= -I ./common

LDFLAGS		+=
LDFLAGS		+= -L .
LDFLAGS		+= -Wl,--build-id=0x$(shell git rev-parse HEAD)

LLVM-S		:= llvm-strip

.PHONY: all
all: $(bin)
	@:

$(bin): $(obj)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
ifeq (0,$(deb))
	$(LLVM-S) -g $@
endif

.PHONY: help
help:
	@echo 'make       - generate the binary file.'
	@echo 'make deb=1 - generate the binary file with debug information'
	@echo 'included (default: deb=0).'
	@echo 'make clean - remove all object, directory and binary files.'

.PHONY: clean
clean:
	@rm -f *.o *.d $(bin)

-include *.d
