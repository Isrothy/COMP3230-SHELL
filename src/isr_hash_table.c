#include "../include/isr_hash_table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct ISRHashTable *
isr_hash_table_new(size_t (*hash_func)(void *), int (*id_func)(void *, void *)) {
    struct ISRHashTable *table = malloc(sizeof(struct ISRHashTable));
    *table = (struct ISRHashTable){
        ISR_HASH_TABLE_INIT_CAP,
        0,
        hash_func,
        id_func,
        malloc(ISR_HASH_TABLE_INIT_CAP * sizeof(struct ISRLinkedList *)),
    };
    for (size_t i = 0; i < table->capacity; ++i) {
        table->lists[i] = isr_linked_list_new();
    }
    return table;
}

int is_prime(size_t x) {
    for (int y = 2; y * y <= x; ++y) {
        if (x % y == 0) {
            return 0;
        }
    }
    return 1;
}

size_t find_next_prime(size_t x) {
    do {
        ++x;
    } while (!is_prime(x));
    return x;
}

void isr_hash_table_resize(struct ISRHashTable *table, size_t new_capacity) {
    struct ISRLinkedList **new_lists = malloc(new_capacity * sizeof(struct ISRLinkedList *));
    for (size_t i = 0; i < new_capacity; ++i) {
        new_lists[i] = isr_linked_list_new();
    }
    for (size_t i = 0; i < table->capacity; ++i) {
        struct ISRLinkedList *l = table->lists[i];
        ISRLinkedListForEach(p, l) {
            struct ISRHashTableEntity *e = p->value;
            // printf("%lu %lu %lu\n", i, e->hash_value, e->hash_value % new_capacity);
            isr_linked_list_insert_tail(new_lists[e->hash_value % new_capacity], e);
        }
        isr_linked_list_free(l, 0);
    }
    // printf("%lu\n", new_capacity);
    free(table->lists);
    table->lists = new_lists;
    table->capacity = new_capacity;
}

void isr_hash_table_insert(struct ISRHashTable *table, void *key, void *value) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    ISRLinkedListForEach(p, l) {
        struct ISRHashTableEntity *o = (struct ISRHashTableEntity *) p->value;
        if (table->id_func(o->key, key)) {
            free(o->value);
            o->value = value;
            return;
        }
    }
    struct ISRHashTableEntity *entity = malloc(sizeof(struct ISRHashTableEntity));
    *entity = (struct ISRHashTableEntity){key, value, hash_value};
    isr_linked_list_insert_tail(table->lists[index], entity);
    ++table->size;
    // printf("%lu %lu\n", hash_value, index);
    if (table->size > table->capacity * ISR_HASH_TABLE_ALPHA) {
        isr_hash_table_resize(table, find_next_prime(table->capacity * 2));
    }
}

void *isr_hash_table_find(struct ISRHashTable *table, void *key) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    ISRLinkedListForEach(p, l) {
        struct ISRHashTableEntity *e = p->value;
        if (table->id_func(e->key, key)) {
            return e->value;
        }
    }
    return NULL;
}


int isr_hash_table_remove(struct ISRHashTable *table, void *key) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    ISRLinkedListForEach(p, l) {
        struct ISRHashTableEntity *e = p->value;
        if (table->id_func(e->key, key)) {
            isr_linked_list_del(l, p);
            free(e);
            --table->size;
            if (table->capacity > 15 && table->size < table->capacity * ISR_HASH_TABLE_BETA) {
                isr_hash_table_resize(table, find_next_prime(table->capacity / 2));
            }
            return 1;
        }
    }
    return 0;
}


int isr_hash_table_contains(struct ISRHashTable *table, void *key) {
    return isr_hash_table_find(table, key) != NULL;
}

void isr_hash_table_free(struct ISRHashTable *table) {
    for (size_t i = 0; i < table->capacity; ++i) {
        isr_linked_list_free(table->lists[i], 1);
    }
    free(table->lists);
    free(table);
}