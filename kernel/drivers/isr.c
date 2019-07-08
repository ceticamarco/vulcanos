#include <stdint.h>
#include "isr.h"
#include "tty.h"
#include "../libc/string.h"
#include "ports.h"

#define PIC1            0x20 // I/O address for master PIC
#define PIC2            0xA0 // I/O address for slave PIC
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC_EOI         0x20 // End Of Interrupt command


// List of messages for known interrupts
uint8_t *interrupts_messages[] = {
    (uint8_t*)"Division by Zero", // 0
    (uint8_t*)"Debug",
    (uint8_t*)"Non-maskable interrupt",
    (uint8_t*)"Breakpoint",
    (uint8_t*)"Detected overflow",
    (uint8_t*)"Out-of-bounds", // 5
    (uint8_t*)"Invalid opcode",
    (uint8_t*)"No coprocessor",
    (uint8_t*)"Double fault",
    (uint8_t*)"Coprocessor segment overrun",
    (uint8_t*)"Bad TSS", // 10
    (uint8_t*)"Segment not present",
    (uint8_t*)"Stack fault",
    (uint8_t*)"General protection fault",
    (uint8_t*)"Page fault",
    (uint8_t*)"Unknown interrupt", // 15
    (uint8_t*)"Coprocessor fault",
    (uint8_t*)"Alignment check",
    (uint8_t*)"Machine check",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved",
    (uint8_t*)"Reserved"
};

isr_t interrupt_handler[256];

void isr_handler(registers_t regs) {
    if(interrupt_handler[regs.int_num] != 0) {
        isr_t handler = interrupt_handler[regs.int_num];
        handler(regs);
    } else {
        uint8_t *buf = (uint8_t*)"\nReceived interrupt: ";
        kprint_c((uint8_t*)buf,strlen(buf), LIGHT_BROWN, BLACK);
        kprint_c(interrupts_messages[(uint8_t)regs.int_num], 
                 strlen(interrupts_messages[(uint8_t)regs.int_num]),
                 WHITE,
                 BLACK
        );
    }
}

void ack_irq(uint32_t int_num) {
    // Send and End Of Interrupt(EOF) at the PICs.
    if(int_num >= 40)
        outb(PIC2_COMMAND, PIC_EOI); // Send reset signal to slave
    outb(PIC1_COMMAND, PIC_EOI); // In any case, reset the master
}

void irq_handler(registers_t regs) {
    ack_irq(regs.int_num);

    if(interrupt_handler[regs.int_num] != 0) {
        isr_t handler = interrupt_handler[regs.int_num];
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handler[n] = handler;
}