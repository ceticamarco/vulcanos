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

void kernel_main() {
    gdt_setup(); // Setup Global Descriptor Table
    idt_setup(); // Setup Interrupt Descriptor Table

    clear_prompt();
    iceos_ascii_logo();
    init_prompt(); // Initialize frame buffer
    init_keyboard(); // Initialize keyboard driver

    // init_timer(1); // Only for debug purposes
}