#ifndef C3230_ISR_DYNAMIC_ARRAY
#define C3230_ISR_DYNAMIC_ARRAY

#include <stddef.h>

void isr_dynamic_array_init(void ***arr, size_t capacity);

void isr_dynamic_array_push_back(void ***arr, void *value, size_t *capacity, size_t *length);

size_t isr_dynamic_array_length(void **arr);

int isr_dynamic_array_is_empty(void **arr);

void isr_dynamic_array_free(void **arr);

#endif