#ifndef C3230_ISR_DYNAMIC_ARRAY
#define C3230_ISR_DYNAMIC_ARRAY

#include <stddef.h>
/**
 * @brief implement the dynamic array data structure.
 * Amortization time and space complexity of adding one element to the tail of the array are O(1)
 *
 */

#define ISR_DYNAMIC_ARRAY_INITIAL_CAPACITY 8

/**
 * @brief initialize a dynamic array
 *
 * @param arr pointer to the array
 * @param capacity initial capacity which should be `ISR_DYNAMIC_ARRAY_INITIAL_CAPACITY`
 * @param size initial size which should be 0.
 */
void isr_dynamic_array_init(void ***arr, size_t *capacity, size_t *size);

/**
 * @brief push an element to the back of the array
 * if the capacity is full, reallocate a new space with double capacity to the original array.
 * @param arr pointer to the array
 * @param value the value to be pushed
 * @param capacity should be the new capacity
 * @param size should be the new size
 */
void isr_dynamic_array_push_back(void ***arr, void *value, size_t *capacity, size_t *size);

/**
 * @brief length of the array
 * find the first NULL in the array
 *
 * @param arr
 * @return size_t the length
 */
size_t isr_dynamic_array_length(void **arr);

/**
 * @brief whether the array is empty
 *
 * @param arr
 * @return int non-zero if empty
 */
int isr_dynamic_array_is_empty(void **arr);

/**
 * @brief release the array
 *
 * @param arr
 */
void isr_dynamic_array_free(void **arr);

#endif