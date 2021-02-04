LD = i686-elf-ld
LFLAGS = -melf_i386 -nostdlib -O2 -T link.ld 

all: prepare cpu kernel_code drivers libc userspace mem link iso

prepare:
	@mkdir -p obj/
	@mkdir -p isodir/boot/grub/
	@echo [MKDIR] obj/
	@echo [MKDIR] isodir/boot/grub
	@cp grub.cfg isodir/boot/grub/grub.cfg 
	@echo [CP] grub.cfg isodir/boot/grub.cfg

cpu:
	@echo [DIR] kernel/cpu
	@make -sC kernel/cpu
	@cp kernel/cpu/*.o_asm obj/
	@echo [CP] kernel/cpu/*.o obj/

kernel_code:
	@echo [DIR] kernel/
	@make -sC kernel
	@cp kernel/*.o obj/
	@echo [CP] kernel/*.o obj/

drivers:
	@echo [DIR] kernel/drivers/
	@make -sC kernel/drivers
	@cp kernel/drivers/*.o obj/
	@echo [CP] kernel/drivers/*.o obj/

libc:
	@echo [DIR] kernel/libc/
	@make -sC kernel/libc
	@cp kernel/libc/*.o obj/
	@echo [CP] kernel/libc/*.o obj/

userspace:
	@echo [DIR] kernel/userspace/
	@make -sC kernel/userspace
	@cp kernel/userspace/*.o obj/
	@echo [CP] kernel/userspace/*.o obj/

mem:
	@echo [DIR] kernel/mem
	@make -sC kernel/mem
	@cp kernel/mem/*.o obj/
	@echo [CP] kernel/mem/*.o obj/

link:
	@$(LD) $(LFLAGS) -o isodir/boot/vulcanos.bin obj/*.o obj/*.o_asm
	@echo [LD] Linking blob...

iso:
	@echo [ISO] Creating ISO file...
	@grub-mkrescue isodir -o vulcanos.iso > /dev/null 2>&1

clean:
	@rm -rf obj/ kernel/*.o kernel/cpu/*.o_asm
	@echo [RM] obj
	@echo [RM] kernel/*.o
	@echo [RM] kernel/cpu/*.o_asm
	@rm -rf kernel/userspace/*.o kernel/mem/*.o
	@echo [RM] kernel/userspace/*.o
	@echo [RM] kernel/mem/*.o
	@rm -rf kernel/drivers/*.o kernel/libc/*.o
	@echo [RM] kernel/drivers/*.o
	@echo [RM] kernel/libc/*.o
	@rm -rf vulcanos.iso bochslog.txt commands isodir
	@echo [RM] vulcanos.iso
	@echo [RM] isodir

bochs:
	bochs -f bochs_cfg -q

run:
	qemu-system-x86_64 -cdrom vulcanos.iso -cpu qemu32
	
run-debug:
	qemu-system-x86_64 -cdrom vulcanos.iso -d exec,cpu

run-curses:
	qemu-system-x86_64 -cdrom vulcanos.iso -curses -cpu qemu32
