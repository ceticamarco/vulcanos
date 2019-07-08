#include "shell.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../drivers/tty.h"
#include "../drivers/ports.h"

void helper() {
    puts("\nList of available commands:\n"
         "\nhelp        - Print this helper"
         "\nint         - Test some interrupts"
         "\nbanner      - Show banner"
         "\nclear, cls  - Clear the screen"
         "\nabout       - About this kernel"
         "\nreboot      - Reboot the system"
        );
}

void test_interrupts() {
    // Testing some interrupts
    asm("int $0"); // Division by zero
    asm("int $4"); // Stack overflow
    asm("int $1"); // Page fault 
}

void about() {
    printf_color("\n====== IceOS v0.0.1 (c) 2019 Marco 'icebit' Cetica ======\n\n",
                 LIGHT_CYAN, BLACK);
    printf_color(
        "iceOS is a x86 monolithic kernel written in C from scratch.\n"  
        "This project doesn't aim to be a fully functional operating system\n"
        "with tons of drivers and graphical applications,\nit's just a learning tool" 
        "to teach myself concepts like\nOperating Systems, Computer Architecture and Digital Electronics.\n" 
        "\n\n"
        "iceOS comes with the following features:\n"
        "- Bare metal booting;\n"
        "- VGA driver;\n"
        "- Interrupts implementation;\n"
        "- PIC & PIT implementation;\n"
        "- PS2 driver;\n"
        "- Support for x86 architecture;\n"
        "- GRUB as bootloader;\n",
    LIGHT_GREEN, BLACK);
}

void reboot() {
    uint8_t tmp;
    asm("cli"); // First disable all interrupts

    // Clear keyboard buffers
    do {
        tmp = inb(0x64); // Keyboard interface
        if(check_flag(tmp, 0) != 0)
            inb(0x60); // Clear keyboard data
    } while(check_flag(tmp, 1) != 0);

    outb(0x64, 0xFE); // Reset the CPU
}

void iceos_ascii_logo() {
    printf_color(
"\n\n###  #####  ####### #######  #####\n"
" #  #     # #       #     # #     #\n"
" #  #       #       #     # #       \n"
" #  #       #####   #     #  #####  \n"
" #  #       #       #     #       # \n"
" #  #     # #       #     # #     # \n"
"###  #####  ####### #######  #####  \n"
"\n     (c) Marco Cetica 2019\n",
 LIGHT_MAGENTA, BLACK);
 }

void processCommand(uint8_t *cmd) {
    if(strcmp(cmd, (uint8_t*)"help") == 0)
        helper();
    else if(strcmp(cmd, (uint8_t*)"int") == 0)
        test_interrupts();
    else if(strcmp(cmd, (uint8_t*)"clear") == 0 || strcmp(cmd, (uint8_t*)"cls") == 0)
        clear_prompt();
    else if(strcmp(cmd, (uint8_t*)"about") == 0)
        about();
    else if(strcmp(cmd, (uint8_t*)"banner") == 0)
        iceos_ascii_logo();
    else if(strcmp(cmd, (uint8_t*)"reboot") == 0)
        reboot();
    else
        puts("\nCommand not found!");
}