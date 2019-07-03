OBJS = multiboot.asm.o kernel_load.asm.o ports.asm.o

ASM = nasm
ASMFLAGS = -f elf

all: $(OBJS)
%.asm.o:
	$(ASM) $(ASMFLAGS) $< -o $@