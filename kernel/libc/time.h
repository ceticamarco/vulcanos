/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
/* Get time reading the RTC(real time clock) CMOS on the motherboard */
#ifndef TIME_H
#define TIME_H

#include <stdint.h>

// Define RTC field registers
#define TIME_R_YEAR 0x09
#define TIME_R_MONTH 0x08
#define TIME_R_DAY 0x07
#define TIME_R_HOUR 0x06
#define TIME_R_MINUTE 0x05
#define TIME_R_SECOND 0x04
//#define TIME_R_CENTURY 0x32 

// Define RTC address
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

// Convert BCD encoed values to binary
#define BCD_CONVERTER(n) ((n / 16) * 10 + (n & 0xF))

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} time_t;

time_t cmos_reader();
uint32_t get_time(uint32_t field);

#endif
