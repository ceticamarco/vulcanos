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
*/

#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "tty.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
