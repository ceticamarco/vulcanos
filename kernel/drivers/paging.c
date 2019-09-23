#include "paging.h"

// Macros for bitset algorithms
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Kernel's page directory
page_directory_t *kernel_directory = 0;
// Current page directory
page_directory_t *current_directory = 0;

// Bitset of frames, used or free
uint32_t *frames;
uint32_t nframes;

// From kheap.c
extern uint32_t placement_address;
extern heap_t *kheap;


// Set a bit in the frame bitset
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

// clear a bit in the frame bitset
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

// Test if a bit is set
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

// Find the first three frames
static uint32_t first_frame() {
    for(uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if(frames[i] != 0xFFFFFFFF) { // If nothing is free, exit
            for(uint32_t j = 0; j < 32; j++) {
                uint32_t to_test = 0x1 << j;
                if (!(frames[i]&to_test))
                    return i*4*8+j;
            }
        }
    }
}

void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable) {
    if(page->frame != 0)
        return;
    else {
        uint32_t idx = first_frame();
        if(idx == (uint32_t)-1) {
            // panic
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 1 : 0;
        page->frame = idx;
    }
}

void free_frame(page_t *page) {
    uint32_t frame;
    if(!(frame=page->frame))
        return;
    else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void init_paging() {
    uint32_t mem_end_page = 0x1000000; // Physical address memory(16MiB big)
    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    // Create a page directory
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;

    /*  Map pages in the kernel heap area.
     * We only call get_page and not alloc_frame to create a new page_table_t 
     * only where necessary.*/
    for(int32_t i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    /* We have eto identify map from 0x0 to the end of the use memory
     * so we can use this memory region as if paging was not enabled. */
    int32_t i = 0;
    while(i < placement_address+0x1000) {
        // Kernel code is read only from userspace
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Perform the real allocation of what we have done so far
    for(i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    
    // Register a new ISR to handle page faults
    register_interrupt_handler(14, page_fault);

    // Enable paging
    switch_page_directory(kernel_directory);

    // Set up kernel heap
    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir->tables_physical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // code to enable paging
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir) {
    // Turn address into an index
    address /= 0x1000;
    // Find page table that contains this index
    uint32_t table_idx = address / 1024;
    if(dir->tables[table_idx]) // If current table is already assigned
        return &dir->tables[table_idx]->pages[address%1024];
    else if(make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_p(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tables_physical[table_idx] = tmp | 0x7;
        return &dir->tables[table_idx]->pages[address%1024];
    } else
        return 0;
}

void page_fault(registers_t regs) {
    // Retrieve faulted address from CR2 register
    uint32_t fault_addr;
    asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

    // Retrieve other infos about the error
    int32_t present = !(regs.err_code & 0x1); // Page not present
    int32_t rw = regs.err_code & 0x2;  // Write operation
    int32_t us = regs.err_code & 0x4; // CPU mode(kernel or user mode)
    int32_t reserved = regs.err_code & 0x8;
    int32_t id = regs.err_code & 0x10;

    // Output of those informations
    kprint("Page fault! ( ");
    if(present)
        kprint("present ");
    if(rw)
        kprint("read-only");
    if(us)
        kprint("user-mode");
    if(reserved)
        kprint("reserved");
    kprint(") at 0x");
    kprint_hex(fault_addr);
    kprint("\n");
    PANIC("Page fault");
}