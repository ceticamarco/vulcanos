/**************************************
 *          VulcanOS Kernel           *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019-2021           *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
/*
 * First a bit of theory:
 * Sometimes you want to interrupt the processor from what it is currently doing 
 * and force it to do something more "critical", such as a timer update or a keyboard interrupt
 * request(IRQ) fires. The processor, then, will handle those IRQs activating an 
 * interrupt handler(ISR, Interrupt Service Routines). In order to know where to find those
 * ISRs, the CPU will use the IDT. So we can say that the Interrupt Description Table
 * is another data structure(organized the same way as the GDT) that will provide a list
 * of interrupts handlers(ISRs) to the CPU.
 */

// Reserved bits in IDT entries
#define IDT_FLAG_RESERVED 0x0E

// PIC
#define PIC1                    0x20 // I/O address for master PIC
#define PIC2                    0xA0 // I/O address for slave PIC
#define PIC1_COMMAND            PIC1
#define PIC1_DATA               (PIC1+1)
#define PIC2_COMMAND            PIC2
#define PIC2_DATA               (PIC2+1)
#define PIC1_START_INTERRUPT    0x20 // Master PIC after remapping
#define PIC2_START_INTERRUPT    0x28 // Slave PIC after remapping
#define PIC_EOI                 0x20 // End of interrupt
#define ICW1_ICW4               0x01
#define ICW1_SINGLE             0x02
#define ICW1_INTERVAL4          0x04
#define ICW1_LEVEL              0x08
#define ICW1_INIT               0x10
#define ICW4_8086               0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO               0x02
#define ICW4_BUF_SLAVE          0x08
#define ICW4_BUF_MASTER         0x0C
#define ICW4_SFNM               0x10

/* Interrupt Descriptor Table */
/* idt_flags contains access flag of a single IDT entry
 * | 0 - 4 | 5 - 6 | 7 | 
 * |  res  |  dpl  | p |
 * res: always 0x0E
 * dpl: ring num (0 - 3)
 * p: segment present (bool)
*/
struct idt_flags {
    uint8_t reserved: 5;
    uint8_t dpl: 2;
    uint8_t p: 1;
}__attribute__((packed));
typedef struct idt_flags idt_flags_t;


/* idt_entry contains the value of an IDT entry
 * Each entry is 64 bits(like GDT entry)
 * | 0 - 15 | 16 - 31 | 32 - 39 | 40 - 47 | 48 - 63 |
 * | b low  | seg sel |   res   | flags   |  b high |
 * b low: Lower 16 bits of the base
 * seg sel: Segment selector for code segment
 * res: always 0
 * flags: idt_flags struct
 * b high: Higher 16 bits of the base
*/
struct idt_entry {
    uint16_t base_low;
    uint16_t seg_sel;
    uint8_t reserved;
    idt_flags_t flags;
    uint16_t base_high;
}__attribute__((packed));
typedef struct idt_entry idt_entry_t;

/* Also we have to define a pointer to the data structure
 * This is needed to locate it later */ 
struct idt_ptr {
    uint16_t limit;
    idt_entry_t *base;
}__attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

/* IDT Kernel API */
void idt_setup();

// ISRs method declaration
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
