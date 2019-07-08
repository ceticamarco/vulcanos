/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#include "drivers/tty.h"
#include "drivers/gdt.h"
#include "drivers/idt.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "shell/shell.h"
#include "libc/stdio.h"

void kernel_main() {
    printf_color("\n[STATUS]", LIGHT_GREEN, BLACK);
    printf_color(" - Loading kernel, wait please...", WHITE, BLACK);

    gdt_setup(); // Setup Global Descriptor Table
    printf_color("\n[INFO]", LIGHT_CYAN, BLACK);
    printf_color("   - Loaded GDT", WHITE, BLACK);

    idt_setup(); // Setup Interrupt Descriptor Table
    printf_color("\n[INFO]", LIGHT_CYAN, BLACK);
    printf_color("   - Loaded IDT", WHITE, BLACK);

    init_timer(1); // Initialize PIT driver
    printf_color("\n[INFO]", LIGHT_CYAN, BLACK);
    printf_color("   - Loaded PIT", WHITE, BLACK);

    init_keyboard(); // Initialize keyboard driver
    printf_color("\n[INFO]", LIGHT_CYAN, BLACK);
    printf_color("   - Loaded PS/2 driver", WHITE, BLACK);
    
    iceos_ascii_logo();
    init_prompt(); // Initialize frame buffer

}