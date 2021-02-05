/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#include "drivers/tty.h"
#include "drivers/gdt.h"
#include "drivers/idt.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/initrd.h"
#include "mem/paging.h"
#include "mem/kheap.h"
#include "mem/multiboot.h"
#include "userspace/shell.h"
#include "libc/stdio.h"
#include "libc/panic.h"

#include <stdint.h>

#define PRTOK printf("\n["); printf_color(" OK ", LIGHT_GREEN, BLACK); printf("]"); // Ugly hack to print "[ OK ]"
#define PRTAT printf("\n["); printf_color(" ** ", LIGHT_BROWN, BLACK); printf("]"); // Ugly hack to print "[ * ]"
#define PRTER printf("\n["); printf_color(" ERR ", LIGHT_RED, BLACK); printf("]"); // Ugly hack to print "[ ER ]"

uint32_t initrd_setup( struct multiboot *mboot_ptr) {
    if(mboot_ptr->mods_count < 0) { // Assert that initrd image is available
        PRTER
        puts("   - initrd not found!");
        PANIC("Cannot find initrd");
    }

    extern uint32_t placement_addr;
    uint32_t initrd_location = *((uint32_t*)mboot_ptr->mods_addr);
    uint32_t initrd_end = *(uint32_t*)(mboot_ptr->mods_addr+4);
    placement_addr = initrd_end;

    return initrd_location;
}

void kernel_main(unsigned long magic, uint32_t addr, struct multiboot *mboot_ptr) {
    // First of all, check if we're booted by a Multiboot-compliant boot loader
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        PRTER
        printf("   - Invalid magic number: %x\n", (unsigned)magic);
        PANIC("Invalid multiboot magic number");
    }

    if(addr & 7) {
        PRTER
        printf("   - Unaligned mbi: %x\n", addr);
        PANIC("Unaligned multiboot MBI");
    }

       
    printf("Loading kernel, wait please...");
    
    gdt_setup(); // Setup Global Descriptor Table
    PRTOK
    printf("   - Loaded GDT");

    idt_setup(); // Setup Interrupt Descriptor Table
    PRTOK
    printf("   - Loaded IDT");

    init_timer(1); // Initialize PIT driver
    PRTOK
    printf("   - Loaded PIT");

    init_keyboard(); // Initialize keyboard driver
    PRTOK
    printf("   - Loaded PS/2 driver");

    uint32_t loc = initrd_setup(mboot_ptr); // Setup initial ramdisk

    init_paging(); // Initialize paging
    PRTOK
    printf("   - Loaded Paging");

    // Now we can start initrd
    fs_root = init_ramdisk(loc);
    PRTOK
    printf("    - Loaded Initrd");
    
    PRTAT
    printf("   - Testing heap...\t");

    uint32_t x = kmalloc(32), y = kmalloc(32);
    printf("x: %x, y: %x", x, y);
    kfree((void*)y);
    uint32_t z = kmalloc(8);
    printf(", z: %x", z); // If z is equal to y, heap's anti-fragmentation algorithm works
    if(z != y) {
        PRTER
        puts("   - Heap test has failed!");
        PANIC("Different heap pages");
    }

    kfree((void*)z), kfree((void*)x);

    PRTOK
    printf("   - Heap works!");

    init_prompt(); // Initialize frame buffer
}
