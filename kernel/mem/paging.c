#include "paging.h"
#include "../libc/string.h"
#include "../libc/assert.h"
#include "../libc/stdio.h"
#include "../drivers/tty.h"

// External definitions from kheap.c
extern uint32_t placement_addr;
extern heap_t *kheap;

// Bitset of frames, used or free
uint32_t *frame_allocations;
uint32_t nframes; // Number of physical frames
page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

static void set_frame(uint32_t addr) {
    uint32_t frame = FRAME(addr);
    uint32_t frame_alloc_s = FRAME_SECTION(frame);
    uint32_t frame_alloc_o = FRAME_OFFSET(frame);
    frame_allocations[frame_alloc_s] |= (1 << frame_alloc_o);
}

static void clear_frame(uint32_t addr) {
    uint32_t frame = FRAME(addr);
    uint32_t frame_alloc_s = FRAME_SECTION(frame);
    uint32_t frame_alloc_o = FRAME_OFFSET(frame);
    frame_allocations[frame_alloc_s] &= ~(1 << frame_alloc_o);
}

static uint32_t first_frame() {
    uint32_t nsections = nframes / FRAME_ALLOCATIONS_SECTION_SIZE;
    for(uint32_t sec = 0; sec < nsections; sec++)
        if(frame_allocations[sec] != USED_FRAME_SECTION)
            for(uint32_t idx = 0; idx < FRAME_ALLOCATIONS_SECTION_SIZE; idx++)
                if(!(frame_allocations[sec] & (0x1 << idx)))
                    return (sec*FRAME_ALLOCATIONS_SECTION_SIZE) + idx;
    return nsections * FRAME_ALLOCATIONS_SECTION_SIZE;
}

void alloc_frame(page_t *page, int32_t is_super, int32_t is_write) {
    if(page->fr != 0)
        return;
    else {
        uint32_t fframe = first_frame();
        if(fframe == (uint32_t)-1) {
            PANIC("No free frames availables!");
        } else {
            // Set free frames to the page
            page->pr = PAGE_PRESENT;
            page->rw = (is_write) ? PAGE_READ_WRITE : PAGE_READ_ONLY;
            page->us = (is_super) ? PAGE_SUPERVISOR : PAGE_USER;
            page->fr = fframe;
            // Set new frames as used
            uint32_t physical_addr = fframe * FRAME_SIZE;
            set_frame(physical_addr);
        }
    }
}

void free_frame(page_t *page) {
    uint32_t frame;
    if(!(frame=page->fr))
        return; // page doesn't have a frame in first place
    else {
        clear_frame(frame);
        page->fr = 0x0;
    }
}

void init_paging() {
    // Setup frame allocation
    nframes = PHYSICAL_MEM_SIZE / FRAME_SIZE;
    frame_allocations = (uint32_t*)kmalloc(nframes/FRAME_ALLOCATIONS_SECTION_SIZE);
    memset(frame_allocations, 0, nframes/FRAME_ALLOCATIONS_SECTION_SIZE);

    // Setup page directory
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    // Map heap pages
    for(uint32_t i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += FRAME_SIZE)
        get_page(i, 1, kernel_directory);

    // Setup identity map
    for(uint32_t i  = 0; i < placement_addr + FRAME_SIZE; i += FRAME_SIZE)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    
    // Allocate heap pages
    for(uint32_t i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += FRAME_SIZE)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

    // Register a new ISR to listen to IRQ 14
    register_interrupt_handler(14, page_fault);
    enable_paging(kernel_directory);
    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void enable_paging(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir->page_tables_physical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Correct code to enable paging
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir) {
    address /= 0x1000; // turn address into an index
    uint32_t table_idx = address / 1024; // Find page that contains the address
    if(dir->page_tables_virtual[table_idx])
        return &dir->page_tables_virtual[table_idx]->pages[address%1024];
    else if(make) {
        uint32_t tmp;
        dir->page_tables_virtual[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->page_tables_virtual[table_idx], 0, 0x1000);
        dir->page_tables_physical[table_idx] = tmp | 0x7;
        return &dir->page_tables_virtual[table_idx]->pages[address%1024];
    } else
        return 0;
}

void page_fault(registers_t regs) {
    // Handle a page fault
    uint32_t faulting_addr;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_addr));

    // Gracefully print the error
    kprint((uint8_t*)"\nPage fault! ( ");
    if(!(regs.err_code & 0x1))
        kprint((uint8_t*)"Present ");
    if(regs.err_code & 0x2)
        kprint((uint8_t*)"Read-Only ");
    if(regs.err_code & 0x4)
        kprint((uint8_t*)"User-Mode ");
    if(regs.err_code & 0x8)
        kprint((uint8_t*)"Reserved");
    printf(") at %x\n", faulting_addr);
    PANIC("Page fault");
}