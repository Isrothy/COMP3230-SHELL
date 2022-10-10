#include "../include/isr_dynamic_array.h"

#include <stdlib.h>

void isr_dynamic_array_init(void ***arr, size_t *capacity, size_t *size) {
    *capacity = ISR_DYNAMIC_ARRAY_INITIAL_CAPACITY;
    *size = 0;
    *arr = (void **) malloc(*capacity * sizeof(void *));
    *arr[0] = NULL;
}

void isr_dynamic_array_push_back(void ***arr, void *value, size_t *capacity, size_t *size) {
    (*arr)[*size] = value;
    ++(*size);
    if (*size + 1 == *capacity) {
        *arr = (void **) realloc(*arr, *capacity * 2 * sizeof(void *));
        *capacity *= 2;
    }
    (*arr)[*size] = NULL;
}

size_t isr_dynamic_array_length(void **arr) {
    size_t len = 0;
    while (arr[len] != NULL) {
        ++len;
    }
    return len;
}

int isr_dynamic_array_is_empty(void **arr) {
    return arr[0] == NULL;
}

void isr_dynamic_array_free(void **arr) {
    free(arr);
}
