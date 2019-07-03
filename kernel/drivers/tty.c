#include "tty.h"
#include "../libc/string.h"
#include "ports.h"

#define VGA_PTR ((uint8_t*) VIDEO_MEM_ADDR) // Pointer to frame buffer
// Also define a 2 byte pointer because cells are 16 bits wide
#define UVGA_PTR ((uint16_t *)VIDEO_MEM_ADDR)

static uint32_t fb_col = 0; // X
static uint32_t fb_row = 0; // Y

void write_cell(int16_t i, int8_t c, uint8_t fg, uint8_t bg) {
    uint8_t *fb = VGA_PTR;
    fb[i*2] = c;
    fb[i*2 + 1] = ((bg & 0x0F) << 4) | (fg | 0x0F);
}

void move_cursor(uint16_t pos) {
    outb(VGA_CMD_PORT, VGA_HIGH_BYTE);
    outb(VGA_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(VGA_CMD_PORT, VGA_LOW_BYTE);
    outb(VGA_DATA_PORT, pos & 0x00FF);
}

void cursor_adv() {
    if(fb_col < VGA_WIDTH - 1)
        fb_col--;
    else
        newline();

    move_cursor(fb_col + (fb_row * VGA_WIDTH));
}

void cursor_prev() {
    if(fb_col == 0) {
        if(fb_row == 0)
            return; // If first row do not do anything
        fb_col = VGA_WIDTH - 1;
        fb_row--;
    } else
        fb_col--;
    move_cursor(fb_col + (fb_row * VGA_WIDTH));
}

void backspace() {
    uint16_t pos;
    uint8_t c = ' ';

    fb_col--;
    pos = fb_col + (fb_row * VGA_WIDTH);
    write_cell(pos, c, WHITE, BLACK);
}

void kprint_c(uint8_t *buf, uint32_t len, uint8_t fg, uint8_t bg) {
    uint16_t pos;
    for(uint32_t i = 0; i < len; i++) {
        uint8_t c = buf[i];
        if(c == '\n' || c == '\r')
            newline();
        else if(c == '\t') {
            pos = fb_col + (fb_row * VGA_WIDTH);
            write_cell(pos, "   ", fg, bg);
            cursor_adv();
        } else {
            pos = fb_col + (fb_row * VGA_WIDTH);
            write_cell(pos, c, fg, bg);
            cursor_adv();
        }
    }
}

void kprint(uint8_t *buf) {
    kprint_c(buf, strlen(buf), WHITE, BLACK);
}

void init_prompt() {
    const uint8_t *prompt = "\nuser@iceOS-$";
    kprint_c(prompt, strlen(prompt), GREEN, BLACK);
}

void clear_prompt() {
    fb_col = 0;
    fb_row = 0;

    for(uint32_t i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++)
        write_cell(i, ' ', WHITE, BLACK);
    move_cursor(0);
}

void clear_row(uint8_t row) {
    for(size_t i = 0; i < VGA_WIDTH; i++)
        write_cell((row*VGA_WIDTH)+i, ' ', WHITE, BLACK);
}

void scroll() {
    uint16_t *fb = UVGA_PTR;
    memmove(fb, fb+VGA_WIDTH, VGA_WIDTH*2*(VGA_HEIGHT*2-1));
    clear_row(VGA_HEIGHT - 1);
}

void newline() {
    if(fb_row < VGA_HEIGHT - 1) // If there's at least one cell add it
        fb_row++;
    else // Otherwise scroll framebuffer
        scroll();

    fb_col = 0;
    move_cursor(fb_col + (fb_row * VGA_WIDTH));
}