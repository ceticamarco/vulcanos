OBJS = multiboot.asm.o kernel_loader.asm.o ports.asm.o  \
		gdt.asm.o idt.asm.o interrupts.asm.o assert.o

ASM = nasm
ASMFLAGS = -f elf
CC = i686-elf-gcc # cross-compiler
CFLAGS = -m32 -fno-stack-protector -ffreestanding -Wall -Wextra -Werror -g -c

all: $(OBJS)
%.asm.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@