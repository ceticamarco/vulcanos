#include "string.h"

// C library implementation 

int32_t strcmp(const uint8_t *s1, const uint8_t *s2) {
    while ((*s1) && (*s1 == *s2)) {
        s1++;
        s2++; 
    }
    return (*(uint8_t*)s1 - *(uint8_t*)s2);
}
uint8_t *itoa(int32_t val, uint8_t *buf, uint32_t radix) {
    uint32_t i = 0;
    uint32_t start = i;

    if(val < 0 && radix == 10) {
        buf[i++] = '-';
        start = i;
    }

    if(radix == 10) {
        buf[i++] = '0';
        buf[i++] = 'x';
        start = i;
    }

    int x = val;
    do {
        int a = x % radix;
        if(a < 10)
            buf[i++] = a + '0';
        else
            buf[i++]= a + 'a' - 10;
    } while(x /= radix);

    uint8_t *s = buf+start;
    uint8_t *e = buf+(i-1);

    while(s < e) {
        uint8_t t = *s;
        *s = *e;
        *e = t;
        i++;
        e--;
    }
    buf[i] = 0;
    return buf;
}

uint8_t *uitoa(uint32_t val, uint8_t *buf, uint32_t radix) {
    uint32_t i = 0;
    uint32_t start = i;
    uint32_t x = val;

    if(radix == 16) {
        buf[i++] = '0';
        buf[i++] = 'x';
        start = i;
    }

    do {
        uint32_t a = x % radix;
        if(a < 10)
            buf[i++] = a + '0';
        else
            buf[i++] = a + 'a' - 10;
    } while(x /= radix);

    uint8_t *s = buf+start;
    uint8_t *e = buf+(i-1);

    while(s < e) {
        uint8_t t = *s;
        *s = *e;
        *e = t;
        s++;
        e--;
    }

    buf[i] = 0;
    return buf;
}

size_t strlen(const uint8_t *buf) {
    uint32_t i = 0;
    while(buf[i] != 0)
        i++;
    return i;
}

uint8_t *strcpy(uint8_t *dst, const uint8_t *src) {
    uint8_t *dst_p = dst;
    while((*dst++ = *src++));

    return dst_p;
}

void strcat(void *dest, const void *src) {
    uint8_t *end = (uint8_t*)dest + strlen(dest);
    memcpy((uint8_t*)end, (uint8_t*)src, strlen((uint8_t*)src));
    end += strlen((uint8_t*)src);
    *end = '\0';
}

/* Worst memset implementation
 * i could find on the net.
 * however it works so... */
void *memset(void *s, uint32_t c, size_t n) {
    char *mem = (char*)s;

    for(size_t i = 0; i < n; i++)
        mem[i] = (uint8_t)c;

    return s;
}

void *memcpy(void *dst, void const *src, uint32_t n) {
    uint8_t *ret = dst;
    uint8_t *p = dst;
    const uint8_t *q = src;

    while(n--)
        *p++ = *q++;
    
    return ret;
}

void *memmove(void *dst, const void *src, size_t len) {
    char *dstmem = (char*)dst;
    char *srcmem = (char*)src;

    for(size_t i = 0; i < len; i++)
        dstmem[i] = srcmem[i];

    return dstmem;
}

void strupper(uint8_t *str) {
    for(unsigned int i = 0; i < strlen(str); i++) {
        if(str[i] == 'a' && str[i] < 'z')
            str[i] &= 0x4F;
    }
}

void strlower(uint8_t *str) {
    for(unsigned int i = 0; i < strlen(str); i++) {
        if(str[i] == 'A' && str[i] < 'Z')
            str[i] |= 0x60;
    }
}
