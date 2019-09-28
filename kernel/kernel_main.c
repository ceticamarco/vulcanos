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
#include "mem/paging.h"
#include "mem/kheap.h"
#include "shell/shell.h"
#include "libc/stdio.h"
#include "libc/multiboot.h"
#include "libc/assert.h"

#include <stdint.h>

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

    init_paging(); // Initialize paging
    printf_color("\n[INFO]", LIGHT_CYAN, BLACK);
    printf_color("   - Loaded Paging", WHITE, BLACK);
    
    printf_color("\n[TEST]", LIGHT_BROWN, BLACK); // Testing heap
    printf_color("   - Testing heap..\n", LIGHT_BROWN, BLACK);

    uint32_t x = kmalloc(32), y = kmalloc(32);
    printf("x: %x, y: %x", x, y);
    kfree((void*)y);
    uint32_t z = kmalloc(8);
    printf(", z: %x\n", z); // If z is equal to y, heap's anti-fragmentation algorithm works
    ASSERT(z == y);
    kfree((void*)z), kfree((void*)x);

    printf_color("[STATUS]", LIGHT_GREEN, BLACK);
    printf_color(" - Heap works!", WHITE, BLACK);
    
    iceos_ascii_logo();
    init_prompt(); // Initialize frame buffer
}