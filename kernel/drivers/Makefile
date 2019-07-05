OBJS = tty.o gdt.o idt.o

CC = i686-elf-gcc # cross-compiler
CFLAGS = -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c


all:${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@