OBJS = multiboot.asm.o kernel_loader.asm.o ports.asm.o

ASM = nasm
ASMFLAGS = -f elf

all: $(OBJS)
%.asm.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@