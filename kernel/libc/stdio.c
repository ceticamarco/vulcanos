#include "stdio.h"
#include "string.h"
#include "drivers/shell.h"


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
                    printk(buf);
                    break;
                case 'x':
                    uval = va_arg(ap, uint32_t);
                    uitoa(uval, buf, 16);
                    printk(buf);
                    break;
                case 'd':
                    uval = va_arg(ap, uint32_t);
                    uitoa(uval, buf, 10);
                    printk(buf);
                    break;
                case 'c':
                    s = va_arg(ap, char*);
                    printk_c(&c, 1);
                    break;
                case 's':
                    s = va_arg(ap, char*);
                    printk(s);
                    break;
                default:
                    printk_c((char*)format+1, 1);
            }
        } else
            printk_c((char*)format+1, 1);
    }
    va_end(ap);
    return 0;
}