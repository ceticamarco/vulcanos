/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "../drivers/isr.h"
#include "kheap.h"

#define FRAME_SIZE 4096
#define PAGE_TABLE_SIZE 1024
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_NOT_PRESENT 0
#define PAGE_PRESENT 1
#define PAGE_READ_ONLY 0
#define PAGE_READ_WRITE 1
#define PAGE_USER 0
#define PAGE_SUPERVISOR 0
#define PAGE_SIZE_4KB 0
#define PAGE_SIZE_4MB 1
// Frames macros
#define FRAME_ALLOCATIONS_SECTION_SIZE 32
#define USED_FRAME_SECTION 0xFFFFFFFF
#define FREE_FRAME_SECTION 0x00000000

#define FRAME(addr) (addr/FRAME_SIZE)
#define FRAME_SECTION(frame) (frame/FRAME_ALLOCATIONS_SECTION_SIZE)
#define FRAME_OFFSET(frame) (frame%FRAME_ALLOCATIONS_SECTION_SIZE)

// Set physical memory to 15 MiB
#define PHYSICAL_MEM_SIZE 0x10000000

struct page { // Single page structure, from intel's developer manual
    uint8_t pr : 1; // Present: 1 to map 4KB page
    uint8_t rw : 1; // Read/Write mode
    uint8_t us : 1; // if 0, user mode access aren't allowed to the page
    uint8_t pw : 1; // Page-level write through
    uint8_t pc : 1; // Page-level cache disable
    uint8_t ac : 1; // 1 if we have accessed 4kb page
    uint8_t di : 1; // 1 if page has been written(dirty)
    uint8_t pa : 1; // Unused bit
    uint8_t gl : 1; // 1 if page is global
    uint8_t ig : 3; // Unused bit
    uint32_t fr: 20; // Physical address of frame
} __attribute__((packed));
typedef struct page page_t;

typedef struct page_table {
    page_t pages[PAGE_TABLE_SIZE];
} page_table_t;

/* Holds 2 arrays for each page directory
 * one holds the physical address, while
 * the other one holds the virtual address
 * (to write/read to it) */
typedef struct page_directory {
    page_table_t *page_tables_virtual[PAGE_DIRECTORY_SIZE];
    uint32_t page_tables_physical[PAGE_DIRECTORY_SIZE];
} page_directory_t;

// Setup environment, page directories and enable paging
void init_paging();
// Perform the "enable-paging" operation to the right register
void enable_paging(page_directory_t *dir);
// Retrieve a pointer from the given page
page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir);
// Identity map(phys = virtual addr) to access it as if paging wasn't enabled
void identity_map();
// Delete a frame
void free_frame(page_t *page);
// Allocate a new frame
void alloc_frame(page_t *page, int32_t is_super, int32_t is_write);
// Page faults handler(ISR recorder)
void page_fault(registers_t regs);

#endif