/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
/*
 * The PIT(Programmable Interval Timer) is a chip that consist of an oscillator
 * It's connected to IRQ0 and it can be configure at a user-defined rate
 * between 10.Hz to 1.1931 MHz. The PIT is the primary method used to implement
 * a system clock and for implement multitasking.
 * The PIT's internal clock(~1.1931 MHz) is fed through a frequency divider that
 * modulate the final signal. This chip has 3 channel, each with his own
 * frequency divider:
 *  Channel 0: The most useful, it's output is connected to IRQ0
 *  Channel 1: It were used to control refresh rates on DRAM(RAMs with capacitors)
 *  Channel 2: Controls the PC speakers.
 * 
 * In our case, we will use only channel 0.
 * So we have to set up PIT at a frequency 'f', so it interrupts us at regular
 * intervals. We'll set the frequency to 100Hz(once every 10 ms); to do this
 * we'll send the PIT a divisor that will be divided for it's input frequency.
 * E.g. -> divisor = 1193180 Hz(1.1931MHz) / 100 Hz
 * 
 * Apart of that, the PIT has 4 registers: 0x40-0x42(data ports) and 0x43(command port)
 */

void init_timer(uint32_t frequency);
extern uint32_t tick;
/* Since regs parameter(from timer_callback) will be unused
 * GCC(with -Werror flag) will throw an error, so we can avoid this
 * using the following macro
 */
#define UNUSED_PAR(x) (void)(x)

#endif
