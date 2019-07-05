/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>
/*
 * First a bit of theory:
 * GDT(Global Descriptor Table) is a complex data structure used in x86 systems
 * to define memory areas.
 * Technically speaking GDT is formed by an array of 8-bytes segment descriptors,
 * the first descriptor of the GDT is always a NULL one and CANNOT be used to allocate
 * memory; so we need at least two descriptors(plus the null descriptor) to successfully allocate
 * data on our memory.
 * In x86 architecture there're two methods to provide virtual memory: Segmentation and Paging:
 * With the first one every memory access is done within his own segment, so each address(of a process) 
 * is added to the segment's base address and checked against the segment's length.
 * With paging, however, the address space is split into blocks(usually of 4KiB) called pages. 
 * Each page can be mapped to physical memory or it can be unmapped(to create virtual memory).
 * Segmentation is a built-in functionality of x86 architecture, so to get around this we need to
 * define our own GDT. A cool thing segmentation can do for us is to set Ring Level: 
 * a privilege level to allow our process to run in a 'unprivileged' mode(called user mode, ring 3)
 * and to allow drivers(or kernel related stuff) to run in a 'supervisor-mode'(called kernel mode, ring 0).
 * Usually bootloader(such as GRUB) sets up GDT for us, the problem is that we cannot known where it is or 
 * if it is has been overwritten during some other tasks. So it's a good idea to implement
 * a new GDT ourself.
 */

/* Those values were taken from Intel's developer manual */

// GDT fields
#define GDT_BASE  0x00000000
#define GDT_LIMIT 0xFFFFFFFF
// GDT granularity
#define GDT_SEGMENT_LENGTH  0xF
#define GDT_OPERAND_SIZE_16 0
#define GDT_OPERAND_SIZE_32 1
#define GDT_GRANULARITY_1K  0
#define GDT_GRANULARITY_4K  1
// GDT access type fields
#define GDT_DATA_TYPE_READ_ONLY                             0x0
#define GDT_DATA_TYPE_READ_ONLY_ACCESSED                    0x1
#define GDT_DATA_TYPE_READ_WRITE                            0x2
#define GDT_DATA_TYPE_READ_WRITE_ACCESSED                   0x3
#define GDT_DATA_TYPE_READ_ONLY_EXPAND_DOWN                 0x4
#define GDT_DATA_TYPE_READ_ONLY_EXPAND_DOWN_ACCESSED        0x5
#define GDT_DATA_TYPE_READ_WRITE_EXPAND_DOWN                0x6
#define GDT_DATA_TYPE_READ_WRITE_EXPAND_DOWN_ACCESSED       0x7
#define GDT_DATA_TYPE_EXEC_ONLY                             0x8
#define GDT_CODE_TYPE_EXEC_ONLY_ACCESSED                    0x9
#define GDT_CODE_TYPE_EXEC_READ                             0xA
#define GDT_CODE_TYPE_EXEC_READ_ACCESSED                    0xB
#define GDT_CODE_TYPE_EXEC_CONFORMING                       0xC
#define GDT_CODE_TYPE_EXEC_CONFORMING_ACCESSED              0xD
#define GDT_CODE_TYPE_EXEC_READ_CONFORMING                  0xE
#define GDT_CODE_TYPE_EXEC_READ_CONFORMING_ACCESSED         0xF
// Descriptor type fields
#define GDT_SYSTEM_DESCRIPTOR        0
#define GDT_CODE_AND_DATA_DESCRIPTOR 1
// GDT Ring number
#define GDT_RING0   0
#define GDT_RING1   1
#define GDT_RING2   2
#define GDT_RING3   3
// 'Present' field
#define GDT_SEGMENT_NOT_PRESENT 0
#define GDT_SEGMENT_PRESENT     1


/* Global Descriptor Table (GDT) implementation */
/* gdt_access is used to access portion of the GDT
 * | 0 - 3 | 4  | 3 - 6 | 7 |
 * | Type  | DT |  DPL  | P |
 * Type: Which type
 * DT: descriptor type
 * DPL: Kernel ring(0-3)
 * P: is segment present? (bool)
 */
struct gdt_access {
    uint8_t type: 4; // 4 Byte
    uint8_t dt: 1; // 1 Byte
    uint8_t dpl: 2; // 2 Byte
    uint8_t p: 1; // 1 Byte
}__attribute__((packed));
typedef struct gdt_access gdt_access_t;

/* gdt_granularity is used to get portion of GDT entry
 * | 0 - 3  | 4 | 5 | 6 | 7 |
 * | seglen |   0   | D | G |
 * seglen: segment length 
 * 0: Always zero
 * D: Operand size (0 = 16 bit, 1 = 32 bit)
 * G: granularity  (0 = 1 Byte, 1 = 4KiB)
 */
struct gdt_granularity {
    uint8_t seglen: 4;
    uint8_t zero: 2;
    uint8_t d: 1;
    uint8_t g: 1;
}__attribute__((packed));
typedef struct gdt_granularity gdt_gran_t;

/* gdt_entry_struct contains the value of a single GDT entry
 *  Each slice is 64 bits.
 *  | 0 - 15 | 16 - 31 | 32 - 39 | 40 - 47 | 48 - 55 | 56 - 63 | 
 *  | lim low| base low|base mid | access  |   gran  | base hg |
 *  lim low: Lower 16 bits of the limit
 *  base low: Lower 16 bits of the base
 *  base mid: Next 8 bits of the base
 *  access: access flag, e.g. which ring this segment can be used in.
 *  gran.
*/
struct gdt_entry_struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    gdt_access_t access;
    gdt_gran_t granularity;
    uint8_t base_high;
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

/* Also we have to define a pointer to the data structure
 * This is needed to locate it later */ 
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

/* GDT Kernel API */
void gdt_setup();


#endif