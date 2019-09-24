/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/


/*** Heap implementation from James Molloy's tutorial:
 * http://www.jamesmolloy.co.uk/tutorial_html/7.-The%20Heap.html ***/



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

#define KHEAP_START 0xC0000000 // starting location can be different
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000

#include <stdint.h>
#include "../cpu/assert.h"
#include "ordered_list.h"
#include "paging.h"

// Data structure for single block/hole
typedef struct {
    uint32_t magic; // Magic number for error checking
    uint8_t is_hole; // 1 if its an hole, 0 for a block
    uint32_t size; // Size of the block
} header_t;

typedef struct {
    uint32_t magic;
    header_t *header; // Pointer to the head
} footer_t;

typedef struct {
    ordered_list_t index;
    uint32_t start_address; // Start of allocated space
    uint32_t end_address; // End of allocated space
    uint32_t max_address; // Maximum size, heap can be expanded to
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;

// Heap functions
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
void free(void *p, heap_t *heap);
// Public heap functions
uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys);
void kfree(void *p);
uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
uint32_t kmalloc(uint32_t sz);

#endif