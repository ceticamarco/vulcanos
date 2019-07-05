/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _ISR_H_
#define _ISR_H_

#include <stdint.h>

/*
 * When we implement ISRs we have to keep in mind that the first 32 interrupts(and so the
 * first 32 ISRs) are reserved by the CPU to signal the kernel about critical actions, 
 * such as divide-by-zero or a stack overflow/buffer overflow.
 * 
 * Below ther's a list of the first, reserved, interrupts...and yeah, we have to implement
 * all of them by ourself(btw in Assembly) :D
 *
 * 0 - Division by zero exception
 * 1 - Debug exception
 * 2 - Non maskable interrupt
 * 3 - Breakpoint exception
 * 4 - Into detected overflow
 * 5 - Out of bounds exception
 * 6 - Invalid opcode exception
 * 7 - No coprocessor exception
 * 8 - Double fault (pushes an error code)
 * 9 - Coprocessor segment overrun
 * 10 - Bad TSS (pushes an error code)
 * 11 - Segment not present (pushes an error code)
 * 12 - Stack fault (pushes an error code)
 * 13 - General protection fault (pushes an error code)
 * 14 - Page fault (pushes an error code)
 * 15 - Unknown interrupt exception
 * 16 - Coprocessor fault
 * 17 - Alignment check exception
 * 18 - Machine check exception
 * 19-31 - Reserved */

#define IRQ0    32
#define IRQ1    33
#define IRQ2    34
#define IRQ3    35
#define IRQ4    36
#define IRQ5    37
#define IRQ6    38
#define IRQ7    39
#define IRQ8    40
#define IRQ9    41
#define IRQ10   42
#define IRQ11   43
#define IRQ12   44
#define IRQ13   45
#define IRQ14   46
#define IRQ15   47

typedef struct registers {
    uint32_t ds; // Data segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed with pusha
    uint32_t int_num, err_code; // Interrupt number and error code
    uint32_t eip, cs, eflags, usereap, ss; // Pushed by CPU
} registers_t;

typedef void (*isr_t)(registers_t);

void ack_irq(uint32_t int_num);
void register_interrupt_handler(uint8_t n, isr_t handler);


#endif
