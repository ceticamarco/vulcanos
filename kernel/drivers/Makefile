OBJS = tty.o gdt.o idt.o isr.o timer.o keyboard.o  \
		fs.o cpuid.o

CC = x86_64-elf-gcc # cross-compiler
CFLAGS = -m32 -fno-stack-protector -DDEFAULT_USER=root -DDEFAULT_HOSTNAME=vulcan -ffreestanding -Wall -Wextra -Werror -g -c


all:${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
