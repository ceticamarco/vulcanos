OBJS = shell.o fetch.o

CC = i686-elf-gcc # cross-compiler
CFLAGS = -DDEFAULT_USER=root -DDEFAULT_HOSTNAME=vulcan -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c 


all:${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
