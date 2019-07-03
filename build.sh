# grub config file
mkdir -p iso_root/boot/grub/
cat > iso_root/boot/grub/grub.cfg << EOF
set timeout = 0
set default = 0

menuentry "iceOS" {
    multiboot2 /boot/kernel.elf
    boot
}
EOF

# CPU
mkdir -p obj/
make -C kernel/cpu
cp kernel/cpu/*.o obj/

# Kernel
make -C kernel
cp kernel/*.o obj/

# Drivers
make -C kernel/drivers/
cp kernel/drivers/*.o obj/

# Libc
make -C kernel/libc
cp kernel/libc/*.o obj/

i686-elf-ld -melf_i386 -nostdlib -O2 -T link.ld -o iso_root/boot/kernel.elf obj/*.o