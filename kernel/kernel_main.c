#include "drivers/tty.h"
#include "libc/stdio.h"

void kernel_main() {
    clear_prompt();
    init_prompt();

    puts("Hello World!");
}