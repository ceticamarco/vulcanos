#include "assert.h"

// We panic when we find a critical error, this function is called by assert macro
extern void panic(const char *message, const char *file, uint32_t line) {
    asm volatile("cli"); // Disable interrupts

    kprint((uint8_t*)"PANIC(");
    kprint((uint8_t*)message);
    kprint((uint8_t*)") at ");
    kprint((uint8_t*)file);
    kprint((uint8_t*)":");
    kprint_dec(line);
    kprint((uint8_t*)"\n");
    // Now hang on for ever
    for(;;);
}

// Check for assertion failed, this function call by assert macro
extern void panic_assert(const char *file, uint32_t line, const char *desc) {
    asm volatile("cli"); // Disable interrupts

    kprint((uint8_t*)"ASSERTION-FAILED(");
    kprint((uint8_t*)desc);
    kprint((uint8_t*)") at ");
    kprint((uint8_t*)file);
    kprint((uint8_t*)":");
    kprint_dec(line);
    kprint((uint8_t*)"\n");
    // Now hang on forever
    for(;;);
}