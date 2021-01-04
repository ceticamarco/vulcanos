OBJS = shell.o fetch.o
VER := $(shell git rev-parse --short HEAD)

CC = i686-elf-gcc # cross-compiler
CFLAGS = -DDEFAULT_USER=root -DDEFAULT_HOSTNAME=vulcan -DVULCAN_VERSION=$(VER) -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c 

all:${OBJS} 

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
