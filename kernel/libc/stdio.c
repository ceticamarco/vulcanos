#include "stdio.h"
#include "string.h"
#include "../drivers/tty.h"

int printf(const char *format, ...) {
    uint8_t buf[20],c,*s;
    int val;
    int32_t uval;
    va_list ap;
    va_start(ap, format);

    for(size_t i = 0; i < strlen((uint8_t*)format); i++) {
        if(format[i] == '%') {
            i++;
            while(format[i] == ' ')
                i++;

            switch(format[i]) {
                case 'i':
                    val = va_arg(ap, int);
                    itoa(val, buf, 10);
                    kprint(buf);
                    break;
                case 'x':
                    uval = va_arg(ap, uint32_t);
                    uitoa(uval, buf, 16);
                    kprint(buf);
                    break;
                case 'd':
                    uval = va_arg(ap, uint32_t);
                    uitoa(uval, buf, 10);
                    kprint(buf);
                    break;
                case 'c':
                    s = va_arg(ap, uint8_t*);
                    kprint_c(&c, 1, WHITE, BLACK);
                    break;
                case 's':
                    s = va_arg(ap, uint8_t*);
                    kprint(s);
                    break;
                default:
                    kprint_c((uint8_t*)format+1, 1, WHITE, BLACK);
            }
        } else
            kprint_c((uint8_t*)format+1, 1, WHITE, BLACK);
    }
    va_end(ap);
    return 0;
}

void puts(const char *buf) {
    printf("%s\n", buf);
}