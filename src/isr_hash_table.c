#include "../include/isr_hash_table.h"
#include <stdio.h>
#include <stdlib.h>

struct ISRHashTable *
isr_hash_table_new(size_t (*hash_func)(void *), int (*id_func)(void *, void *)) {
    struct ISRHashTable *table = malloc(sizeof(struct ISRHashTable));
    table->hash_func = hash_func;
    table->id_func = id_func;
    table->capacity = 7;
    table->size = 0;
    table->lists = malloc(table->capacity * sizeof(struct ISRLinkedList *));
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
        struct ISRLinkedList *list = table->lists[i];
        for (struct ISRLinkedListNode *p = list->sentinal->next; p != NULL; p = p->next) {
            struct ISRHashTableEntity *e = p->value;
            isr_linked_list_insert_tail(new_lists[e->hash_value % new_capacity], e);
        }
        isr_linked_list_free(list, 0);
    }
    free(table->lists);
    table->lists = new_lists;
    table->capacity = new_capacity;
}

void isr_hash_table_insert(struct ISRHashTable *table, void *key, void *value) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *list = table->lists[index];
    for (struct ISRLinkedListNode *p = list->sentinal->next; p != NULL; p = p->next) {
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
    if (table->size > table->capacity * ISR_HASH_TABLE_ALPHA) {
        isr_hash_table_resize(table, find_next_prime(table->capacity * 2));
    }
}

void *isr_hash_table_find(struct ISRHashTable *table, void *key) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    for (struct ISRLinkedListNode *p = l->sentinal->next; p != NULL; p = p->next) {
        struct ISRHashTableEntity *o = p->value;
        if (table->id_func(o->key, key)) {
            return o->value;
        }
    }
    return NULL;
}


int isr_hash_table_remove(struct ISRHashTable *table, void *key) {
    size_t hash_value = table->hash_func(key);
    size_t index = hash_value % table->capacity;
    struct ISRLinkedList *l = table->lists[index];
    for (struct ISRLinkedListNode *p = l->sentinal; p->next != NULL; p = p->next) {
        struct ISRLinkedListNode *q = p->next;
        struct ISRHashTableEntity *o = q->value;
        if (table->id_func(o->key, key)) {
            p->next = q->next;
            free(q->value);
            free(q);
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