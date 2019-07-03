#include "stdio.h"
#include "string.h"
#include "../drivers/tty.h"

int printf(const char *format, ...) {
    char buf[20],c,*s;
    int val;
    int32_t uval;
    va_list ap;
    va_start(ap, format);

    for(size_t i = 0; i < strlen(format); i++) {
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
                    s = va_arg(ap, char*);
                    kprint_c(&c, 1, WHITE, BLACK);
                    break;
                case 's':
                    s = va_arg(ap, char*);
                    kprint(s);
                    break;
                default:
                    kprint_c((char*)format+1, 1, WHITE, BLACK);
            }
        } else
            kprint_c((char*)format+1, 1, WHITE, BLACK);
    }
    va_end(ap);
    return 0;
}

void puts(int8_t *buf) {
    printf("%s\n", buf);
}