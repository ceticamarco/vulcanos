#include "kheap.h"

/* The following function is a simple method to find the smallest hole that
 * fit user space request, since we will do this process many times, it's a good
 * idea to wrap it in a function */
static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap) {
    uint32_t i = 0;
    while(i < heap->index.size) {
        header_t *header = (header_t*)lookup_ordered_list(i, &heap->index);
        if(page_align > 0) {
            uint32_t loc = (uint32_t)header;
            int32_t offset = 0;
            if((loc+sizeof(header_t)) & 0xFFFFF000 != 0) // Page aligned memory
                offset = 0x1000 - (loc+sizeof(header_t))%0x1000;
            int32_t hole_size = (int32_t)header->size - offset;
            if(hole_size >= (int32_t)size)
                break;
        } else if(header->size >= size)
            break;
        i++;
    }
    // Return something according to the iterator
    if(i == heap->index.size)
        return -1; // Nothing found
    else
        return i;
}

static int8_t header_t_less_than(void *a, void *b) {
    return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly) {
    heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t)); //TODO: implement kmalloc

    ASSERT(start%0x1000 == 0);
    ASSERT(end%0x1000 == 0);

    // Init heap's index
    heap->index = place_ordered_list((void*)start, HEAP_INDEX_SIZE, &header_t_less_than);

    // Shift start address to the corrent position, where we can put on data
    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    // Check if start address is page-aligned
    if(start & 0xFFFFF000 != 0) {
        start &= 0xFFFFF000;
        start += 0x1000;
    }

    // Fill the heap structure with start, end and max addresses
    heap->start_address = start;
    heap->end_address = end;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    // Let's create one large hole in the new index
    header_t *hole = (header_t*)start;
    hole->size = end-start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_list((void*)hole, &heap->index);

    return heap;
}

static void expand(uint32_t new_size, heap_t *heap) {
    // Before anything else let's check that new size is greater than older one
    ASSERT(new_size > heap->end_address - heap->start_address);
    if(new_size&0xFFFFF000 != 0) {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }
    // Check if new size is not greater than maximum size
    ASSERT(heap->start_address+new_size <= heap->max_address);

    uint32_t old_size = heap->end_address-heap->start_address;
    uint32_t i = old_size;
    while(i < new_size) {
        alloc_frame(get_page(heap->start_address+i, 1, kernel_directory), // FIXME:
        (heap->supervisor)?1:0, (heap->readonly)?0:1);
    }
}

static uint32_t contract(uint32_t new_size, heap_t *heap) {
    // This function will be literally the opposite of the previous one
    ASSERT(new_size < heap->end_address-heap->start_address);

    if(new_size&0x1000) {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    if(new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;
    uint32_t old_size = heap->end_address-heap->start_address;
    uint32_t i = old_size - 0x1000;
    while(new_size < i) {
        free_frame(get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
    return new_size;
}

