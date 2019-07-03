iso:
	grub-mkrescue iso_root -o iceOS.iso

run:
	qemu-system-x86_64 -cdrom iceOS.iso

bochs:
	bochs -f bochs_cfg -q

clean:
	rm -rf obj/ kernel/*.o kernel/cpu/*.o iso_root/boot/kernel.elf
	rm -rf kernel/drivers/*.o kernel/libc/*.o
	rm -rf iceOS.iso bochslog.txt commands iso_root
	rm -rf .vscode
