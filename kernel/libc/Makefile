OBJS = stdio.o string.o panic.o time.o
VER := $(shell git rev-parse --short HEAD)

CC = i686-elf-gcc # cross-compiler
CFLAGS = -DVULCAN_VERSION=$(VER) -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c


all:${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
