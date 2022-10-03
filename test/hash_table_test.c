#include "../include/isr_hash_table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t hash_func(void *key) {
    size_t k = *(int *) key;
    return (k << 56) ^ (k << 48) ^ (k << 40) ^ (k << 32) ^ (k << 24) ^ (k << 16) ^ (k << 8) ^ k;
}

int id_func(void *key1, void *key2) {
    return *((int *) key1) == *((int *) key2);
}

int main() {
    struct ISRHashTable *table = isr_hash_table_new(hash_func, id_func);
    int k[20] = {1, 2, 3, 4, 7, 8, 10, 123, 192, 608, 17, 99, 12, 33};
    int v[20] = {8, 40, -45, 22, 99, 8, 10, 123, 2, 233, 2333, -2, 98, 15};

    for (int i = 0; i < 14; ++i) {
        int *key = malloc(sizeof(int));
        *key = k[i];
        int *value = malloc(sizeof(int));
        *value = v[i];
        isr_hash_table_insert(table, key, value);
    }

    for (int i = 0; i < 14; ++i) {
        assert(isr_hash_table_contains(table, &k[i]));
        int *value = isr_hash_table_find(table, &k[i]);
        assert(*value == v[i]);
    }

    int *key = malloc(sizeof(int));
    *key = 8;
    int *value = malloc(sizeof(int));
    *value = 233;
    isr_hash_table_insert(table, key, value);
    assert(*((int *) isr_hash_table_find(table, key)) == *value);

    for (int i = 0; i < 11; ++i) {
        isr_hash_table_remove_dum(table, &k[i]);
    }
    for (int i = 0; i < 11; ++i) {
        assert(!isr_hash_table_contains(table, &k[i]));
    }
    for (int i = 11; i < 14; ++i) {
        assert(isr_hash_table_contains(table, &k[i]));
        int *value = isr_hash_table_find(table, &k[i]);
        assert(*value == v[i]);
    }


    return 0;
}