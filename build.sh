# grub config file
mkdir -p isodir/boot/grub/
cat > isodir/boot/grub/grub.cfg << EOF
set timeout = 0
set default = 0

menuentry "iceOS" {
    multiboot2 /boot/iceOS.bin
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

# Shell
make -C kernel/shell
cp kernel/shell/*.o obj/

# Linking
i686-elf-ld -melf_i386 -nostdlib -O2 -T link.ld -o isodir/boot/iceOS.bin obj/*.o

# ISO
make iso