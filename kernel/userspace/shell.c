#include "shell.h"
#include "fetch.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../drivers/tty.h"
#include "../drivers/ports.h"
#include "../drivers/timer.h"

void helper() {
    puts("\nList of available commands:\n"
         "\nhelp        - Print this helper"
         "\nint         - Test some interrupts"
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
    printf_color("\n             \\                  /,\n", LIGHT_RED, BLACK);
    printf_color("    _________))                ((__________\n", LIGHT_RED, BLACK);
    printf_color("   /.-------./\\\\    \\    /    //\\.--------.\\", LIGHT_RED, BLACK);
    printf_color("  Vulcan", LIGHT_MAGENTA, BLACK);
    printf_color("OS ", LIGHT_BROWN, BLACK);
    printf_color("is x86 monolithic \n", LIGHT_GREEN, BLACK);
    printf_color("  //#######//##\\\\   ))  ((   //##\\\\########\\\\", LIGHT_RED, BLACK);
    printf_color("  kernel written from scratch \n", LIGHT_GREEN, BLACK);
    printf_color(" //#######//###((  ((    ))  ))###\\\\########\\\\", LIGHT_RED, BLACK);
    printf_color("  in C following the UNIX \n", LIGHT_GREEN, BLACK);
    printf_color("((#######((#####\\\\  \\\\  //  //#####))########))", LIGHT_RED, BLACK);
    printf_color("  philosophy. \n", LIGHT_GREEN, BLACK);
    printf_color(" \\##' `###\\######\\\\  \\)(/  //######/####' `##/", LIGHT_RED, BLACK);
    printf_color("    By Marco Cetica 2019-2021 \n", LIGHT_GREEN, BLACK);
    printf_color("  )'    ``#)'  `##\\`->oo<-'/##'  `(#''     `(", LIGHT_RED, BLACK);
    printf_color("      https://git.io/JtzuAs\n", WHITE, BLACK);
    printf_color("          (       ``\\`..'/''       )\n", LIGHT_RED, BLACK);
    printf_color("                     \\''(\n", LIGHT_RED, BLACK);
    printf_color("                      `- )\n", LIGHT_RED, BLACK);
    printf_color("                      / /\n", LIGHT_RED, BLACK);
    printf_color("                     ( /\\\n", LIGHT_RED, BLACK);
    printf_color("                     /\\| \\\n", LIGHT_RED, BLACK);
    printf_color("                    (  \\\n", LIGHT_RED, BLACK);
    printf_color("                        )\n", LIGHT_RED, BLACK);
    printf_color("                       /\n", LIGHT_RED, BLACK);
    printf_color("                      (\n", LIGHT_RED, BLACK);
    printf_color("                      `\n", LIGHT_RED, BLACK);

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
    else if(strcmp(cmd, (uint8_t*)"fetch") == 0)
        system_fetcher();
    else if(strcmp(cmd, (uint8_t*)"reboot") == 0)
        reboot();
    else if(strcmp(cmd, (uint8_t*)"") == 0)
        puts("");
    else
        puts("\nCommand not found!");
}
