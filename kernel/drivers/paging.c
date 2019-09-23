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