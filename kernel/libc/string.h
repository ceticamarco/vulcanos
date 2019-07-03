/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/

#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h> // For uinx_t
#include <stddef.h> // For size_t

int32_t strcmp(const char *s1, const char *s2);
char *itoa(int val, char *buf, int radix);
char *uitoa(uint32_t val, char *buf, int radix);
size_t strlen(const char *buf);
void *memset(void *s, int c, size_t n);
void *memmove(void *dst, const void *src, size_t len);
void strupper(char *str);
void strlower(char *str);

#endif
