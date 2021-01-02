/**************************************
 *          VulcanOS Kernel           *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019-2021           *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/

/*** Heap implementation from James Molloy's tutorial:
  http://www.jamesmolloy.co.uk/tutorial_html/7.-The%20Heap.html ***/

/* This heap algorithm uses two different data structures: blocks and holes.
 * Blocks: Contiguous areas of memory containing user data
 * Holes: Special kind of blocks that are not in use, this is the result
 * of free() operation. Those spaces lend to a common problem called "Fragmentation";
 * where malloc() cannot use those spaces anymore because they are too small for any
 * kind of program. Any modern OS must have a solution to avoid this problem, but to keep
 * things simple as possible i wont implement anything like that.
 * Blocks/holes contains informations like the magic number(error checking), the type of 
 * chunk(hole or block) and the size, while the footer contains only a pointer to the header
 * (and obviously an error checking flag).
 */
#ifndef KHEAP_H
#define KHEAP_H

#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000

#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000

#include <stdint.h>
#include "ordered_array.h"

// Data structure for single hole/block
typedef struct {
    uint32_t magic; // Magic number for error checking
    uint8_t is_hole; // 1 if it's an hole, 0 for a block
    uint32_t size; // Size of block
} header_t;

typedef struct {
    uint32_t magic; // Same as above
    header_t *header; // Pointer to the header block
} footer_t;

typedef struct {
    ordered_array_t index;
    uint32_t start_address; // Begin of allocated space
    uint32_t end_address; // End of allocated space
    uint32_t max_address; // Maximum size heap can be expanded to
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;

// Create a new heap
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
// Allocates a contigious region of memory in size
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
// Free a block allocated with alloc
void free(void *p, heap_t *heap);
uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys);
uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
uint32_t kmalloc(uint32_t sz);
void kfree(void *p);

#endif
