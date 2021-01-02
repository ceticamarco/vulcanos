#include "time.h"
#include "../drivers/ports.h"

// Check whether CMOS is updated or not
static uint8_t is_cmos_updated() {
    outb(CMOS_ADDRESS, 0x0A);
    
    return (inb(CMOS_DATA) & 0x80);
}

// Get CMOS register's status
static uint8_t reg_status(int32_t reg) {
    outb(CMOS_ADDRESS, reg);
    
    return inb(CMOS_DATA);
}


time_t cmos_reader() {
    while(is_cmos_updated()); // Wait until the CMOS is being updated
    
    time_t tm;

    tm.second = BCD_CONVERTER(reg_status(TIME_R_SECOND));
    tm.minute = BCD_CONVERTER(reg_status(TIME_R_MINUTE));
    tm.hour = BCD_CONVERTER(reg_status(TIME_R_HOUR));
    tm.day = BCD_CONVERTER(reg_status(TIME_R_DAY));
    tm.month = BCD_CONVERTER(reg_status(TIME_R_MONTH));
    tm.year = BCD_CONVERTER(reg_status(TIME_R_YEAR));

    return tm;
}

uint32_t get_time(uint32_t field) {
    while(is_cmos_updated()); // Wait the CMOS is being updated

    return BCD_CONVERTER(reg_status(field));
}
