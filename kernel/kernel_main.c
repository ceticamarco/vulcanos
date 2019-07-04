#include "drivers/tty.h"
#include "drivers/gdt.h"
#include "libc/stdio.h"

void kernel_main() {
    clear_prompt();
    init_prompt(); // Initialize frame buffer
    gdt_setup(); // Setup Global Descriptor Table

    puts("Hello World!");
}