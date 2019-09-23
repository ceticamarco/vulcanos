#ifndef PAGING_H
#define PAGING_G

#include <stdint.h>
#include "isr.h"
#include "tty.h"
#include "kheap.h"

typedef struct page {
    uint32_t present : 1; // Presence in memory of single page
    uint32_t rw : 1; // if set read write otherwise read only
    uint32_t user : 1;  // Supervisored mode only if cleared
    uint32_t accessed : 1; // true if page has been accessed since last refresh
    uint32_t dirty : 1; // Same as above for write.
    uint32_t unused : 7; // unused bits
    uint32_t frame : 20; // Frame address (shifted to the right)
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    page_table_t *tables[1024]; // array of pointers to page tables
    uint32_t tables_physical[1024]; // address of tables physical. 
    uint32_t physical_addr;
} page_directory_t;

// Setup the environment
void init_paging();
// Load specified page directory into CR3 register
void switch_page_directory(page_directory_t *new);
// Retrieve pointer to specified page address
page_t *get_page(uint32_t addr, int32_t make, page_directory_t *dir);
// Handle page faults
void page_fault(registers_t regs);

#endif