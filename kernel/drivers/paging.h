#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "isr.h"

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
#define FRAME_ALLOCATION_SECTION_SIZE 32
#define USED_FRAME_ALLOCATIONS_SECTION 0xFFFFFFFF
#define FREE_FRAME_ALLOCATIONS_SECTION 0x00000000

// Reserve 16 MiB of physical memory
#define PHYSICAL_MEMORY_SIZE 0x10000000

struct page { // Page structure from Intel's developer manual
    uint8_t present : 1;
    uint8_t rw : 1;
    uint8_t user : 1;
    uint8_t pwt : 1;
    uint8_t pcd : 1;
    uint8_t a : 1;
    uint8_t d : 1;
    uint8_t pat : 1;
    uint8_t g : 1;
    uint8_t unused : 3;
    uint32_t frame : 20;
} __attribute__((packed));
typedef struct page page_t;

struct page_table {
    page_t pages[PAGE_TABLE_SIZE];
};
typedef struct page_table page_table_t;

/* For each page we hold two arrays:
 * one is used by the CPU to hold the physical address
 * the other is used to hold the virtual address to actual read
 * or write to it. */
struct page_directory {
    page_table_t *page_table_virtual[PAGE_DIRECTORY_SIZE];
    uint32_t page_table_physical[PAGE_DIRECTORY_SIZE];
};
typedef struct page_directory page_directory_t;

// Setup the environment
void init_paging();
// Load specified page directory into CR3 register
void enable_paging(page_directory_t *new);
// Retrieve pointer to specified page address
page_t *get_page(uint32_t addr, int32_t make, page_directory_t *dir);
// Handle page faults
void page_fault(registers_t regs);
// Allocate a new frame
void alloc_frame(page_t *page, int32_t is_supervisored, int32_t is_writeable);
// Deallocate frame
void free_frame(page_t *page);

#endif