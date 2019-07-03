/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/

#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>

// VGA colors
enum TTY_COLORS {
    BLACK, // 0
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE // 15
};

/* Framebuffer properties  */
#define VIDEO_MEM_ADDR 0x000B8000 // frame buffer address
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* VGA I/O ports */
#define VGA_CMD_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

/* VGA I/O ports commands */
#define VGA_HIGH_BYTE 14
#define VGA_LOW_BYTE 15

/* Kernel's VGA API */
void write_cell(int16_t i, int8_t c, uint8_t fg, uint8_t bg);
void move_cursor(uint16_t pos);
void cursor_adv();
void backspace();
void kprint_c(uint8_t *buf, uint32_t len, uint8_t fg, uint8_t bg);
void kprint(uint8_t *buf);
void init_prompt();
void clear_prompt();
void clear_row(uint8_t row);
void scroll(); // Scroll one row
void newline();

#endif