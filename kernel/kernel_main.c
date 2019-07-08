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
#include "libc/stdio.h"

void kernel_main() {
    gdt_setup(); // Setup Global Descriptor Table
    idt_setup(); // Setup Interrupt Descriptor Table

    clear_prompt();
    init_prompt(); // Initialize frame buffer
    //puts("Hello World!");
    init_timer(1);

    /*
    // Testing some interrupts
    asm("int $0"); // Division by zero
    asm("int $4"); // Stack overflow
    asm("int $1"); // Page fault 
    */
}