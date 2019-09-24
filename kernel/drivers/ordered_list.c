#include "ordered_list.h"

uint8_t standard_lessthan_predicate(type_t a, type_t b) {
    if(a < b)
        return 1;
    return 0;
}

// Create an ordered list
ordered_list_t create_ordered_list(uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_list_t to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_then = less_than;
    return to_ret;
}

ordered_list_t place_ordered_list(void *addr, uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_list_t to_ret;
    to_ret.array = (type_t*)addr;
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_then = less_than;
    return to_ret;
}

// Destroy an ordered list
void destroy_ordered_list(ordered_list_t *array) {
    kfree(array->array);
}

// Insert item into the array
void insert_ordered_list(type_t item, ordered_list_t *array) {
    ASSERT(array->less_then);
    uint32_t iterator = 0;
    while(iterator < array->size && array->less_then(array->array[iterator], item))
        iterator++;
    if(iterator == array->size)
        array->array[array->size++] = item;
    else {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while(iterator < array->size) {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

// Find item at given index
type_t lookup_ordered_list(uint32_t i, ordered_list_t *array) {
    ASSERT(i < array->size);
    return array->array[i];
}

// Delete item from the array
void remove_ordered_list(uint32_t i, ordered_list_t *array) {
    while(i < array->size) {
        array->array[i] = array->array[i+1];
        i++;
    }
    array->size--;
}