/**************************************
 *          VulcanOS Kernel           *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019-2021           *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

int printf(const char *format, ...);
int printf_color(const char *format, uint8_t fg, uint8_t bg); // Only for string for now
void puts(const char *buf);

#endif
