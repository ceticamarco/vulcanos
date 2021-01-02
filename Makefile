LD = i686-elf-ld
LFLAGS = -melf_i386 -nostdlib -O2 -T link.ld 

all: prepare cpu kernel_code drivers libc userspace mem link iso

prepare:
	mkdir -p obj/
	mkdir -p isodir/boot/grub/
	cp grub.cfg isodir/boot/grub/grub.cfg 

cpu:
	make -C kernel/cpu
	cp kernel/cpu/*.o obj/

kernel_code:
	make -C kernel
	cp kernel/*.o obj/

drivers:
	make -C kernel/drivers/
	cp kernel/drivers/*.o obj/

libc:
	make -C kernel/libc
	cp kernel/libc/*.o obj/

userspace:
	make -C kernel/userspace
	cp kernel/userspace/*.o obj/

mem:
	make -C kernel/mem
	cp kernel/mem/*.o obj/

link:
	$(LD) $(LFLAGS) -o isodir/boot/iceOS.bin obj/*.o

iso:
	grub-mkrescue isodir -o iceOS.iso

clean:
	rm -rf obj/ kernel/*.o kernel/cpu/*.o
	rm -rf kernel/shell/*.o kernel/mem/*.o
	rm -rf kernel/drivers/*.o kernel/libc/*.o
	rm -rf iceOS.iso bochslog.txt commands isodir

bochs:
	bochs -f bochs_cfg -q

run:
	qemu-system-x86_64 -cdrom iceOS.iso
	
run-debug:
	qemu-system-x86_64 -cdrom iceOS.iso -d exec,cpu

run-curses:
	qemu-system-x86_64 -cdrom iceOS.iso -curses
