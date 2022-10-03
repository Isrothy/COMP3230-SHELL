#ifndef C3230_ISR_HASH_TABLE
#define C3230_ISR_HASH_TABLE

#include "isr_linked_list.h"
#include <stddef.h>

#define ISR_HASH_TABLE_ALPHA (0.75)
#define ISR_HASH_TABLE_BETA (0.25)

struct ISRHashTableEntity {
    void *key;
    void *value;
    size_t hash_value;
};

struct ISRHashTable {
    size_t capacity;
    size_t size;
    size_t (*hash_func)(void *);
    int (*id_func)(void *, void *);
    struct ISRLinkedList **lists;
};

struct ISRHashTable *
isr_hash_table_new(size_t (*hash_func)(void *), int (*id_func)(void *, void *));

void isr_hash_table_insert(struct ISRHashTable *table, void *key, void *value);

int isr_hash_table_remove(struct ISRHashTable *table, void *key);

void *isr_hash_table_find(struct ISRHashTable *table, void *key);

int isr_hash_table_contains(struct ISRHashTable *table, void *key);

void isr_hash_table_free(struct ISRHashTable *table);

#endif