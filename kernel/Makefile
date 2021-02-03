OBJS = kernel_main.o

CC = i686-elf-gcc # cross-compiler
CFLAGS = -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c


all: ${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@