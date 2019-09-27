#include "kheap.h"
#include "paging.h"

// TODO: add assert

extern uint32_t end;
uint32_t placement_addr = (uint32_t)&end;
extern page_directory_t *kernel_directory;
heap_t *kheap = 0;

uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys) {
    if(kheap != 0) {
        void *addr = alloc(sz, (uint8_t)align, kheap);
        if(phys != 0) {
            page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
            *phys = page->fr*0x1000 + ((uint32_t)addr&0xFFF);
        }
        return (uint32_t)addr;
    } else {
        if(align == 1 && (placement_addr & 0xFFFFF000)) {
            placement_addr &= 0xFFFFF000;
            placement_addr += 0x1000;
        }
        if(phys)
            *phys = placement_addr;
        uint32_t tmp = placement_addr;
        placement_addr += sz;
        return tmp;
    }
}

void kfree(void *p) {
    free(p, kheap);
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
    return kmalloc_int(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
    return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz) {
    return kmalloc_int(sz, 0, 0);
}

static void expand(uint32_t new_size, heap_t *heap) {
    // Get nearest page boundary
    if((new_size&0xFFFFF000) != 0) {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    uint32_t old_size = heap->end_adddress - heap->start_address;

    uint32_t it = old_size;
    while(it < new_size) {
        alloc_frame(get_page(heap->start_address+it, 1, kernel_directory),
        (heap->supervisor) ? 1 : 0, (heap->readonly) ? 0 : 1);
        it += 0x1000; // Page size
    }
    heap->end_adddress = heap->start_address+new_size;
}

static uint32_t contract(uint32_t new_size, heap_t *heap) {
    if(new_size&0x1000) {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    if(new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;
    
    uint32_t old_size = heap->end_adddress - heap->start_address;
    uint32_t it = old_size - 0x1000;
    while(new_size < it) {
        free_frame(get_page(heap->start_address+it,0, kernel_directory));
        it -= 0x1000;
    }

    heap->end_adddress = heap->start_address + new_size;
    return new_size;
}

static uint32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap) {
    uint32_t it = 0;

    // Find smallest hole that fit our request
    while(it < heap->index.size) {
        header_t *head = (header_t*)lookup_ordered_array(it, &heap->index);
        if(page_align > 0) {
            // IF page must be aligned
            uint32_t location = (uint32_t)head;
            uint32_t offset = 0;
            if((((location+sizeof(header_t)) & 0xFFFFF000) != 0))
                offset = 0x1000 - (location+sizeof(header_t))%0x1000;
            uint32_t hole_size = (uint32_t)head->size - offset;
            // Check if we can fit this page in that hole
            if(hole_size >= (uint32_t)size)
                break;
        } else if(head->size >= size)
            break;
        it++;
    }

    // If we didn't find anything
    if(it == heap->index.size)
        return -1;
    else
        return it;
}

static uint8_t header_t_less_than(void *a, void *b) {
    return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t *create_heap(uint32_t start, uint32_t end_addr, uint32_t max, uint8_t supervisor, uint8_t readonly) {
    heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t));

    // Initialize the index
    heap->index = place_ordered_array((void*)start, HEAP_MIN_SIZE, &header_t_less_than);
    // Shift start address to the right
    start += sizeof(type_t) * HEAP_INDEX_SIZE;

    // Check if start address is page-aligned
    if ((start & 0xFFFFF000) != 0) {
        start &= 0xFFFFF000;
        start += 0x1000;
    }
    // Store vars into heap
    heap->start_address = start;
    heap->end_adddress = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    header_t *hole = (header_t*)start;
    hole->size = end - start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_array((void*)hole, &heap->index);

    return heap;
}

void *alloc(uint32_t size, uint8_t page_align, heap_t *heap) {
    uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
    // Find smallest hole suitable
    uint32_t it = find_smallest_hole(new_size, page_align, heap);

    if((int32_t)it == -1) {
        uint32_t old_len = heap->end_adddress - heap->start_address;
        uint32_t old_end_addr = heap->end_adddress;

        // Allocate more space
        expand(old_len+new_size, heap);
        uint32_t new_len = heap->end_adddress - heap->start_address;

        it = 0;
        uint32_t idx = -1; uint32_t value = 0x0;
        while(it < heap->index.size) {
            uint32_t tmp = (uint32_t)lookup_ordered_array(it, &heap->index);
            if(tmp > value) {
                value = tmp;
                idx = it;
            }
            it++;
        }

        // If no headers has been found, add a new one
        if((int32_t)idx == -1) {
            header_t *head = (header_t*)old_end_addr;
            head->magic = HEAP_MAGIC;
            head->size = new_len - old_len;
            head->is_hole = 1;
    
            footer_t *foot = (footer_t*)(old_end_addr + head->size - sizeof(footer_t));
            foot->magic = HEAP_MAGIC;
            foot->header = head;
            insert_ordered_array((void*)head, &heap->index);
        } else {
            header_t *head = lookup_ordered_array(idx, &heap->index);
            head->size += new_len - old_len;
            // Update the footer
            footer_t *foot = (footer_t*)((uint32_t)head + head->size - sizeof(footer_t));
            foot->header = head;
            foot->magic = HEAP_MAGIC;
        }        
        // Now we have enough space, so recall this function again
        return alloc(size, page_align, heap);
    }
}