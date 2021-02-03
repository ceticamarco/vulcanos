/* The keyboard driver works with a device called PS/2: to talk with this
 * controller we can use serial communication(e.g. ports.h). The actual flow of 
 * data is made with "commands", each command is one byte and the keyboard's 
 * controller can send two type of response:
 * ACK(Acknowledge): to acknowledge the previous command
 * Resend: to resend the previous command due to an error.
 * We have also to wait between the command, the data and the response of
 * the PS/2 controller.
 * This device should not exists anymore in any modern computer 
 * motherboard; however the CPU(or the motherboard?) should be able to 
 * simulate it even if we're using some USB keyboard.
 * Apart of that the keyboard will be listen on IRQ1(33),
 * so we have to register an ISR for that.
*/

#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "tty.h"
#include "../userspace/shell.h"
#include "../libc/stdio.h"

static void keyboard_callback();

/* Keyboard scan codes map , layout is: standard US keyboard */
uint8_t keyboard_scan_codes[] = {
    0, (uint8_t)27, (uint8_t)'1', (uint8_t)'2', (uint8_t)'3', (uint8_t)'4', (uint8_t)'5', (uint8_t)'6', (uint8_t)'7', (uint8_t)'8',
    (uint8_t)'0', (uint8_t)'0', (uint8_t)'-', (uint8_t)'=', (uint8_t)'\b',
    (uint8_t)'\t',
    (uint8_t)'q', (uint8_t)'w', (uint8_t)'e', (uint8_t)'r',
    (uint8_t)'t', (uint8_t)'y', (uint8_t)'u', (uint8_t)'i', (uint8_t)'o', (uint8_t)'p', (uint8_t)'[', (uint8_t)']', (uint8_t)'\n',
    0,
    (uint8_t)'a', (uint8_t)'s', (uint8_t)'d', (uint8_t)'f', (uint8_t)'g', (uint8_t)'h', (uint8_t)'j', (uint8_t)'k', (uint8_t)'l', (uint8_t)';',
    (uint8_t)'\'', (uint8_t)'`', 0,
    (uint8_t)'\\', (uint8_t)'z', (uint8_t)'x', (uint8_t)'c', (uint8_t)'v', (uint8_t)'b', (uint8_t)'n',
    (uint8_t)'m', (uint8_t)',', (uint8_t)'.', (uint8_t)'/', 0,
    (uint8_t)'*',

    0, // Alt
  (uint8_t)' ', // Spacebar
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
   (uint8_t)'-',
    0, // Left arrow
    0, 
    0, // Right arrow
   (uint8_t)'+',
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

uint8_t command[512]; // Max length of a single command
uint8_t temp[512];
uint32_t cmd_index = 0;
uint32_t shiftdown = 0;

void clear() {
    for(uint16_t i = 0; i < 512; i++)
        command[i] = 0;
}

void clear_tmp(void) {
    for(uint16_t i = 0; i < 512; i++)
        command[i] = 0;
}

static void keyboard_callback() {
    uint8_t scan_code = inb(KB_DATA_PORT); // Read from keyboard
    uint8_t keycode = keyboard_scan_codes[scan_code]; // Get ASCII value

    if(scan_code & 0x80) {
        if(scan_code == 0xAA || scan_code == 0x86)
            shiftdown = 0;
    } else {
        // Handle backspace
        if(keycode == 0x08) {
            if(cmd_index <= 0) // If at start of the prompt
                return; // Do not delete it.
            cmd_index--; // Otherwise go back

        for(uint32_t i = 0; i < cmd_index; ++i)
            temp[i] = command[i];
        clear();
        for(uint32_t i = 0; i < cmd_index; ++i)
            command[i] = temp[i];
        clear_tmp();
        backspace();
        } else if(keycode == 0x0A) { // Handle Enter
            processCommand(command);
            cmd_index = 0;
            clear();
            init_prompt();
        } else {
            printf("%c", keycode);
            command[cmd_index] = keycode;
            cmd_index++;
        }
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
