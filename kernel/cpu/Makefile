SOURCES = main.asm ports.asm  \
		gdt.asm idt.asm \
		interrupts.asm header.asm
OBJS = $(SOURCES:.asm=.o_asm)
ASM = nasm
ASMFLAGS = -f elf32

all: $(OBJS)

%.o_asm: %.asm
	@$(ASM) $(ASMFLAGS) $< -o $@
	@echo [ASM] compiling $<...