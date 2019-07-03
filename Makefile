iso:
	grub-mkrescue isodir -o iceOS.iso

run:
	qemu-system-x86_64 -cdrom iceOS.iso

bochs:
	bochs -f bochs_cfg -q

clean:
	rm -rf obj/ kernel/*.o kernel/cpu/*.o
	rm -rf kernel/drivers/*.o kernel/libc/*.o
	rm -rf iceOS.iso bochslog.txt commands isodir
