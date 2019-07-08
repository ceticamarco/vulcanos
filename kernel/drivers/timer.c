#include "timer.h"
#include "isr.h"
#include "tty.h"
#include "../libc/string.h"
#include "ports.h"

// Start tick at zero
uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    uint8_t buf[8];
    itoa(tick, buf, 10);
    kprint((uint8_t*)"Time: ");
    uitoa(tick, buf, 10);
    kprint((uint8_t*)buf);
    kprint((uint8_t*)"\n");

    // Cast to void unused parameter
    UNUSED_PAR(regs);
}

void init_timer(uint32_t frequency) {
    // Register a new ISR for IRQ0
    register_interrupt_handler(IRQ0, &timer_callback);

    /* As mentioned before, we'll send to PIT a value to divide for
     * his system clock(1.1931 MHz). We have to keep in mind that
     * this value must fit into a 16 bits variable */
    uint32_t divisor = 1193180 / frequency;

    // Send command to the right port
    outb(0x43, 0x36);
    // The two divisor has to be sent byte-wise, to do this we split them in two parts
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor
    outb(0x40, low);
    outb(0x40, high);
}