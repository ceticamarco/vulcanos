/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>

/* Our list is always in a 'sorted state',
 * it can store anything that can be casted
 * to void* */
typedef void* type_t;
/* The following predicate should return non-zero
 * if the first argument is less than the second */
typedef uint8_t (*lessthan_predicate_t)(type_t,type_t);
typedef struct {
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

uint8_t standard_lessthan_predicate(type_t a, type_t b);

// Create a new ordered array
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);
// Destroy an ordered array
void destroy_ordered_array(ordered_array_t *array);
// Add an item into the array
void insert_ordered_array(type_t item, ordered_array_t *array);
type_t lookup_ordered_array(uint32_t i, ordered_array_t *array);
void remove_ordered_array(uint32_t i, ordered_array_t *array);

#endif
