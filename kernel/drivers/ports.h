/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#ifndef _PORTS_H
#define _PORTS_H

#include <stdint.h>

/* outb:
 * Redirect data to port(high level interface for ports.asm)
 * @param port: Output port to send data to.
 * @param data: The actual data to send to port
*/
void outb(uint16_t port, uint16_t data);

/* inb:
 * Fetch data from a port, return a char
 * @param port: Input port to read data from.
*/
uint8_t inb(uint16_t port);


#endif
