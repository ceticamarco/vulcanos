#include "fetch.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/time.h"
#include "../drivers/tty.h"
#include "../drivers/cpuid.h"
#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

static void get_date();
static void get_cpuid();
static void get_version();

void system_fetcher() {
    uint8_t user[64], hostname[64];
#ifdef DEFAULT_USER
        strcpy(user, (uint8_t*)STRINGIZE_VALUE_OF(DEFAULT_USER));
#else
        #error "-DDEFAULT_USER flag not set"
#endif

#ifdef DEFAULT_HOSTNAME
        strcpy(hostname, (uint8_t*)STRINGIZE_VALUE_OF(DEFAULT_HOSTNAME));
#else
        #error "-DDEFAULT_HOSTNAME flag not set"
#endif
    // print first line
    printf_color("\n __   __   ___  ___    ", LIGHT_RED, BLACK);
    printf_color((char*)user, LIGHT_CYAN, BLACK);
    printf_color("@", LIGHT_RED, BLACK);
    printf_color((char*)hostname, LIGHT_CYAN, BLACK);
    // print second line
    printf_color("\n \\ \\ / /  / _ \\/ __|   ", LIGHT_RED, BLACK);
    printf_color("-----------", LIGHT_RED, BLACK);
    // print third line
    printf_color("\n  \\ V /  | (_) \\__ \\   ", LIGHT_RED, BLACK);
    printf_color("Date: ", LIGHT_CYAN, BLACK);
    get_date();
    // print fourth line
    printf_color("\n   \\_/    \\___/|___/   ", LIGHT_RED, BLACK);
    get_cpuid();
    // print VulcanOS version
    get_version();
}

// Date format is D/M/YY
void get_date() {
    printf("%d", get_time(TIME_R_DAY)); // Day
    printf_color("/", LIGHT_BLUE, BLACK);

    printf("%d", get_time(TIME_R_MONTH)); // Month
    printf_color("/", LIGHT_BLUE, BLACK);

    printf("%d", get_time(TIME_R_YEAR)); // Year
}


void get_cpuid() {
    printf_color("CPU type: ", LIGHT_CYAN, BLACK);
    printf("%s", (char*)get_cpu_type()); // Print CPU type
    printf("%s%s%c", " (", (char*)get_cpu_family(), ')');
}


void get_version() {
    uint8_t version[64];
#ifdef VULCAN_VERSION
    strcpy(version, (uint8_t*)STRINGIZE_VALUE_OF(VULCAN_VERSION));
#else
    #error "-DVULCAN_VERSION flag not set"
#endif
    
    printf_color("\n\t\t\t\t\t   Version: ", LIGHT_CYAN, BLACK);
    printf_color((char*)version, LIGHT_GREEN, BLACK);
}
