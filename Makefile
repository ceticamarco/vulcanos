iso:
	grub-mkrescue iso_root -o iceOS.iso

run:
	qemu-system-x86_64 -cdrom iceOS.iso

clean:
	rm -rf obj/ kernel/*.o kernel/cpu/*.o iso_root/boot/kernel.elf
	rm -rf kernel/drivers/*.o kernel/libc/*.o
	rm -rf PyramidKernel.iso bochslog.txt commands