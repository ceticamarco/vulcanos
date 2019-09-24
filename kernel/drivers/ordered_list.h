/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include <stdint.h>

/* Our list is always in a 'sorted state',
 * it can store anything that can be casted
 * to void* */
typedef void* type_t;
/* The following predicate should return non-zero
 * if the first argument is less than the second */
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);
typedef struct {
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_then;
} ordered_list_t;
uint8_t standard_lessthan_predicate(type_t a, type_t b);

// Create an ordered list
ordered_list_t create_ordered_list(uint32_t max_size, lessthan_predicate_t less_than);
ordered_list_t place_ordered_list(void *addr, uint32_t max_size, lessthan_predicate_t less_than);
// Destroy an ordered list
void destroy_ordered_list(ordered_list_t *array);
// Insert item into the array
void insert_ordered_list(type_t item, ordered_list_t *array);
// Find item at given index
type_t lookup_ordered_list(uint32_t i, ordered_list_t *array);
// Delete item from the array
void remove_ordered_list(uint32_t i, ordered_list_t *array);

#endif