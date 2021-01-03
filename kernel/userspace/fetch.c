#include "fetch.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/time.h"
#include "../drivers/tty.h"



void system_fetcher() {
    /*uint8_t *logo = (uint8_t*)" \n  \
    __   __   ___  ___  \n \
     \\ \\ / /  / _ \\/ __| \n \
      \\ V /  | (_) \\__ \\ \n \
       \\_/    \\___/|___/";*/

    uint8_t user[64], hostname[64];
#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)
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
    printf_color("\n__   __   ___  ___ ", LIGHT_RED, BLACK);
    printf_color((char*)user, LIGHT_CYAN, BLACK);
    printf_color("@", LIGHT_RED, BLACK);
    printf_color((char*)hostname, LIGHT_CYAN, BLACK);
}
