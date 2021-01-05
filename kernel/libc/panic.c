#include "panic.h"
#include "../drivers/cpuid.h"
#include "../libc/stdio.h"
#include "../libc/string.h"

#define KINFO printf("["); printf_color(" I ", LIGHT_RED, BLACK); printf("]: ");
#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)


// We panic when we find a critical error, this function is called by assert macro
extern void panic(const char *message, const char *file, uint32_t line) {
    uint8_t version[64];
#ifdef VULCAN_VERSION
    strcpy(version, (uint8_t*)STRINGIZE_VALUE_OF(VULCAN_VERSION));
#else
    #error "-DVULCAN_VERSION flag not set"
#endif
    printf_color("=============================================\n", LIGHT_MAGENTA, BLACK);
    printf_color("          .:: KERNEL PANIC ::.  \n", LIGHT_RED, BLACK);
    printf_color("Reason: ", LIGHT_BROWN, BLACK);
    printf("'%s' at '%s':%d\n", message, file, line);
    KINFO
    printf_color("Disabling interrupts\n", LIGHT_GREEN, BLACK);
    asm volatile("cli"); // Disable interrupts
    KINFO
    printf_color("Dropping you into an endless loop\n", LIGHT_GREEN, BLACK);
    KINFO
    printf_color("Your are on your own now...good luck.\n", LIGHT_GREEN, BLACK);
    KINFO
    printf_color("VulcanOS version: ", LIGHT_GREEN, BLACK);
    printf_color((char*)version, LIGHT_CYAN, BLACK);
    printf_color("\n\t\t (c) 2019-2021 Marco Cetica", LIGHT_BROWN, BLACK);
    printf_color("\n=============================================\n", LIGHT_MAGENTA, BLACK);
    for(;;);
}

// Check for assertion failed, this function is called by assert macro
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
