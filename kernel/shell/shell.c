#include "shell.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../drivers/tty.h"
#include "../drivers/ports.h"
#include "../drivers/timer.h"

void helper() {
    puts("\nList of available commands:\n"
         "\nhelp        - Print this helper"
         "\nint         - Test some interrupts"
         "\nbanner      - Show banner"
         "\nclear, cls  - Clear the screen"
         "\nregs        - Prints register dump"
         "\ntimer       - Prints timer tick"
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
        "with tons of drivers and graphical applications,\nit's just a learning tool " 
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

void register_dump() {
    register uint32_t eax_v asm("eax");
    register uint32_t ebx_v asm("ebx");
    register uint32_t ecx_v asm("ecx");
    register uint32_t edx_v asm("edx");
    register uint32_t esx_v asm("esi");
    register uint32_t edi_v asm("edi");
    register uint32_t ebp_v asm("ebp");
    register uint32_t esp_v asm("esp");

    printf_color("\n===================================\n"
                 "  BEGIN 32 BITS CPU REGISTER DUMP    \n"
                 "===================================\n",
                 LIGHT_BROWN, BLACK);
    printf("    EAX: %x\n"
           "    EBX: %x\n"
           "    ECX: %x\n"
           "    EDX: %x\n"
           "    ESX: %x\n"
           "    EDI: %x\n"
           "    EBP: %x\n"
           "    ESP: %x\n",
           eax_v, ebx_v, ecx_v, edx_v, esx_v, edi_v, ebp_v, esp_v);
    printf_color("\n==================================\n"
                 "   END 32 BITS CPU REGISTER DUMP    \n"
                 "==================================\n",
                 LIGHT_BROWN, BLACK);
}

void timer_dump() {
    uint8_t buf[8];
    uitoa(tick, buf, 10);

    printf_color("\nTicks since boot: ",
        LIGHT_GREEN, BLACK);
    printf_color((const char*)buf,
        LIGHT_CYAN, BLACK);
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

void processCommand(uint8_t *cmd) {
    if(strcmp(cmd, (uint8_t*)"help") == 0)
        helper();
    else if(strcmp(cmd, (uint8_t*)"int") == 0)
        test_interrupts();
    else if(strcmp(cmd, (uint8_t*)"clear") == 0 || strcmp(cmd, (uint8_t*)"cls") == 0)
        clear_prompt();
    else if(strcmp(cmd, (uint8_t*)"about") == 0)
        about();
    else if(strcmp(cmd, (uint8_t*)"regs") == 0)
        register_dump();
    else if(strcmp(cmd, (uint8_t*)"timer") == 0)
        timer_dump();
    else if(strcmp(cmd, (uint8_t*)"banner") == 0)
        iceos_ascii_logo();
    else if(strcmp(cmd, (uint8_t*)"reboot") == 0)
        reboot();
    else
        puts("\nCommand not found!");
}

void iceos_ascii_logo() {
printf_color(
    "\n\n"
    " MMMMMMMMMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
    " MMMMMMMMMMMM...?MMM.. MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
    " MMMMMMMMM ........M .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
    " MMMMMMMM...........M .MMMMMMMM..MMO ........MMM....MM..MMMMM\n"
    " MMMMM ..............M..MMMMMMM..M...........M.....,.M.. MMMM\n"
    " MMMM................MM,.. .MMM....MMMM......M..MMMM..M,..MMM\n"
    " MMM ................... MMMMMM....MMMM......M..MMMM..MMN..MM\n"
    " M ......................,MMMMM..M.. ........M.......M... .~M\n"
    " ........................MMMMM..MMM.........MMM...MMM....MMMM\n"
    " .........................MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
    " M .....................DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
    " MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n",
    LIGHT_MAGENTA, BLACK);
    printf_color("                     (c) Marco Cetica 2019\n", LIGHT_GREEN, BLACK);
 }