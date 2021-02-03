#include "idt.h"
#include "../libc/string.h"
#include "ports.h"

// Internal method
extern void idt_flush(idt_ptr_t*); // defined on cpu/idt.asm
static void init_idt();
static void idt_set_gate(uint8_t idx, void(*base), uint16_t selector, idt_flags_t flags);
static void pic_remap(uint8_t offset1, uint8_t offset2);

idt_entry_t idt_entries[256]; // 256 interrupts
idt_ptr_t idt_ptr;

// This method will be called by the kernel
void idt_setup() {
    init_idt();
}

static void idt_set_gate(uint8_t idx, void(*base), uint16_t selector, idt_flags_t flags) {
    idt_entries[idx] = (struct idt_entry) {
        .base_low = (uint32_t)base & 0xFFFF,
        .base_high = ((uint32_t)base >> 16) & 0xFFFF,
        .seg_sel = selector,
        .flags = flags
    };
}

static void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    idt_flags_t flags = {
        .reserved = IDT_FLAG_RESERVED, // Always 0x0E
        .dpl = 0,
        .p = 1
    };
    // Remap IDT with ISRs
    idt_set_gate(0, isr0, 0x08, flags);
    idt_set_gate(1, isr1, 0x08, flags);
    idt_set_gate(2, isr2, 0x08, flags);
    idt_set_gate(3, isr3, 0x08, flags);
    idt_set_gate(4, isr4, 0x08, flags);
    idt_set_gate(5, isr5, 0x08, flags);
    idt_set_gate(6, isr6, 0x08, flags);
    idt_set_gate(7, isr7, 0x08, flags);
    idt_set_gate(8, isr8, 0x08, flags);
    idt_set_gate(9, isr9, 0x08, flags);
    idt_set_gate(10, isr10, 0x08, flags);
    idt_set_gate(11, isr11, 0x08, flags);
    idt_set_gate(12, isr12, 0x08, flags);
    idt_set_gate(13, isr13, 0x08, flags);
    idt_set_gate(14, isr14, 0x08, flags);
    idt_set_gate(15, isr15, 0x08, flags);
    idt_set_gate(16, isr16, 0x08, flags);
    idt_set_gate(17, isr17, 0x08, flags);
    idt_set_gate(18, isr18, 0x08, flags);
    idt_set_gate(19, isr19, 0x08, flags);
    idt_set_gate(20, isr20, 0x08, flags);
    idt_set_gate(21, isr21, 0x08, flags);
    idt_set_gate(22, isr22, 0x08, flags);
    idt_set_gate(23, isr23, 0x08, flags);
    idt_set_gate(24, isr24, 0x08, flags);
    idt_set_gate(25, isr25, 0x08, flags);
    idt_set_gate(26, isr26, 0x08, flags);
    idt_set_gate(27, isr27, 0x08, flags);
    idt_set_gate(28, isr28, 0x08, flags);
    idt_set_gate(29, isr29, 0x08, flags);
    idt_set_gate(30, isr30, 0x08, flags);
    idt_set_gate(31, isr31, 0x08, flags);

    // Remap PIC
    pic_remap(PIC1_START_INTERRUPT, PIC2_START_INTERRUPT);

    // Also remap 15 entries for IRQs
    idt_set_gate(32, irq0, 0x08, flags);
    idt_set_gate(33, irq1, 0x08, flags);
    idt_set_gate(34, irq2, 0x08, flags);
    idt_set_gate(35, irq3, 0x08, flags);
    idt_set_gate(36, irq4, 0x08, flags);
    idt_set_gate(37, irq5, 0x08, flags);
    idt_set_gate(38, irq6, 0x08, flags);
    idt_set_gate(39, irq7, 0x08, flags);
    idt_set_gate(40, irq8, 0x08, flags);
    idt_set_gate(41, irq9, 0x08, flags);
    idt_set_gate(42, irq10, 0x08, flags);
    idt_set_gate(43, irq11, 0x08, flags);
    idt_set_gate(44, irq12, 0x08, flags);
    idt_set_gate(45, irq13, 0x08, flags);
    idt_set_gate(46, irq14, 0x08, flags);
    idt_set_gate(47, irq15, 0x08, flags);

    idt_flush(&idt_ptr);

    // Finally enable hardware interrupts with an assembly instruction
    __asm__ __volatile__ ("sti");
}

// Taken from: http://wiki.osdev.org/8259_PIC
static void pic_remap(uint8_t offset1, uint8_t offset2) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA); // Save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4); // Start init sequence
    outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);

    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);

    outb(PIC1_DATA, 4); // Tell master PIC that there is a slave PIC at IRQ2
    outb(PIC1_DATA, 2); // Tell salve PIC it's cascade identity

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}