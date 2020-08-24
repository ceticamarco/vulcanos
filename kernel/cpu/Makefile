OBJS =  kernel_loader.asm.o ports.asm.o  \
		gdt.asm.o idt.asm.o interrupts.asm.o

ASM = nasm
ASMFLAGS = -f elf32
all: $(OBJS)
%.asm.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@
