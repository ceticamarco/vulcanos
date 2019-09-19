#include "assert.h"

// We panic when we find a critical error, this function is called by assert macro
extern void panic(const char *message, const char *file, uint32_t line) {
    asm volatile("cli"); // Disable interrupts

    kprint("PANIC(");
    kprint(message);
    kprint(") at ");
    kprint(file);
    kprint(":");
    kprint_dec(line);
    kprint("\n");
    // Now hang on for ever
    for(;;);
}

// Check for assertion failed, this function call by assert macro
extern void panic_assert(const char *file, uint32_t line, const char *desc) {
    asm volatile("cli"); // Disable interrupts

    kprint("ASSERTION-FAILED(");
    kprint(desc);
    kprint(") at ");
    kprint(file);
    kprint(":");
    kprint_dec(line);
    kprint("\n");
    // Now hang on forever
    for(;;);
}