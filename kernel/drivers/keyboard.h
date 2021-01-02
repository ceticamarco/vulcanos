/**************************************
 *          VulcanOS Kernel           *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019-2021           *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define KB_DATA_PORT 0x60 // Keyboard serial port

void init_keyboard();

#endif
