/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#ifndef CPUID_H
#define CPUID_H

#include <stdint.h>
#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b"(b), "=d" (d) : "a" (in));

typedef struct {
    uint32_t model;
    uint32_t family;
    uint32_t type;
    uint32_t brand;
    uint32_t stepping;
    uint32_t reserved;
} icpuid_t;

typedef struct {
    uint8_t *model;
    uint8_t *family;
    uint8_t *type;
    uint8_t *brand;
    uint8_t *stepping;
    uint8_t *reserved;
} cpuid_t;

// return type and family processor
uint8_t *get_cpu_type();
uint8_t *get_cpu_family();

#endif
