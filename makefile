bin		:= usha256

src		:=
src		+= main.c

dep		:= ${bin:=.d}

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

all: $(bin)
	@:

$(bin): $(src)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	@rm -f $(bin) $(dep)
