#include "../include/isr_hash_table.h"
#include <stdlib.h>

struct ISRHashTable *
isr_hash_table_new(size_t (*hash_func)(void *), int (*id_func)(void *, void *)) {
    struct ISRHashTable *table = malloc(sizeof(struct ISRHashTable));
    *table = (struct ISRHashTable){
        ISR_HASH_TABLE_INIT_CAP,
        0,
        hash_func,
        id_func,
        (struct ISRLinkedList **) malloc(ISR_HASH_TABLE_INIT_CAP * sizeof(struct ISRLinkedList *)),
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
    struct ISRLinkedList **new_lists
        = (struct ISRLinkedList **) malloc(new_capacity * sizeof(struct ISRLinkedList *));
    for (size_t i = 0; i < new_capacity; ++i) {
        new_lists[i] = isr_linked_list_new();
    }
    for (size_t i = 0; i < table->capacity; ++i) {
        struct ISRLinkedList *l = table->lists[i];
        ISRLinkedListForEach(p, l) {
            struct ISRHashTableEntity *e = p->value;
            isr_linked_list_insert_tail(new_lists[e->hash_value % new_capacity], e);
        }
        isr_linked_list_free(l, 0);
    }
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
    struct ISRHashTableEntity *entity
        = (struct ISRHashTableEntity *) malloc(sizeof(struct ISRHashTableEntity));
    *entity = (struct ISRHashTableEntity){key, value, hash_value};
    isr_linked_list_insert_tail(table->lists[index], entity);
    ++table->size;
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


struct ISRHashTableEntity *isr_hash_table_remove(struct ISRHashTable *table, void *key) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    ISRLinkedListForEach(p, l) {
        struct ISRHashTableEntity *e = p->value;
        if (table->id_func(e->key, key)) {
            isr_linked_list_del(l, p);
            --table->size;
            if (table->capacity > 15 && table->size < table->capacity * ISR_HASH_TABLE_BETA) {
                isr_hash_table_resize(table, find_next_prime(table->capacity / 2));
            }
            return e;
        }
    }
    return NULL;
}

void isr_hash_table_remove_dum(struct ISRHashTable *table, void *key) {
    struct ISRHashTableEntity *e = isr_hash_table_remove(table, key);
    if (e != NULL) {
        free(e);
    }
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