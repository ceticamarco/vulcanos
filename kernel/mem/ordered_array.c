#include "ordered_array.h"
#include "kheap.h"
#include "../libc/panic.h"
#include "../libc/string.h"

uint8_t standard_lessthan_predicate(type_t a, type_t b) {
    if (a < b)
        return 1;
    else 
        return 0;
}

ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_array_t to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_array_t to_ret;
    to_ret.array = (type_t*)addr;
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void destroy_ordered_array(ordered_array_t *array) {
    kfree(array->array);
}

void insert_ordered_array(type_t item, ordered_array_t *array) {
    uint32_t it = 0;
    while(it < array->size && array->less_than(array->array[it], item))
        it++;
    if(it == array->size)
        array->array[array->size++] = item;
    else {
        type_t tmp = array->array[it];
        array->array[it] = item;
        while(it < array->size) {
            it++;
            type_t tmp2 = array->array[it];
            array->array[it] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookup_ordered_array(uint32_t i, ordered_array_t *array) {
    return array->array[i];
}

void remove_ordered_array(uint32_t i, ordered_array_t *array) {
    while(i < array->size) {
        array->array[i] = array->array[i+1];
        i++;
    }
    array->size--;
}
