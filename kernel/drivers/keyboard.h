/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

void init_keyboard();

/* Keyboard scan codes map */
uint8_t *keyboard_scan_codes[] = {
    0, (uint8_t*)27, (uint8_t*)'1', (uint8_t*)'2', (uint8_t*)'3', (uint8_t*)'4', (uint8_t*)'5', (uint8_t*)'6', (uint8_t*)'7', (uint8_t*)'8',
    (uint8_t*)'0', (uint8_t*)'0', (uint8_t*)'-', (uint8_t*)'=', (uint8_t*)'\b',
    (uint8_t*)'\t',
    (uint8_t*)'q', (uint8_t*)'w', (uint8_t*)'e', (uint8_t*)'r',
    (uint8_t*)'t', (uint8_t*)'y', (uint8_t*)'u', (uint8_t*)'i', (uint8_t*)'o', (uint8_t*)'p', (uint8_t*)'[', (uint8_t*)']', (uint8_t*)'\n',
    0,
    (uint8_t*)'a', (uint8_t*)'s', (uint8_t*)'d', (uint8_t*)'f', (uint8_t*)'g', (uint8_t*)'h', (uint8_t*)'j', (uint8_t*)'k', (uint8_t*)'l', (uint8_t*)';',
    (uint8_t*)'\'', (uint8_t*)'`', 0,
    (uint8_t*)'\\', (uint8_t*)'z', (uint8_t*)'x', (uint8_t*)'c', (uint8_t*)'v', (uint8_t*)'b', (uint8_t*)'n',
    (uint8_t*)'m', (uint8_t*)',', (uint8_t*)'.', (uint8_t*)'/', 0,
    (uint8_t*)'*',

    0, // Alt
  (uint8_t*)' ', // Spacebar
    0, // Caps lock
    0, // 59 - F1 key
    0, // 59 - F1 key
    0, 0, 0, 0, 0, 0, 0, 0,
    0, // F10
    0, // 69 - Num lock
    0, // Scroll lock
    0, // Home key
    0, // Up arrow
    0, // Page up
   (uint8_t*)'-',
    0, // Left arrow
    0, 
    0, // Right arrow
   (uint8_t*)'+',
    0, // 79 End key
    0, // Down arrow 
    0, // Page down
    0, // Insert key
    0, // Delete key
    0, 0, 0,
    0, // F11 key
    0, // F12 key
    0 // Others key are undefined
};

#endif