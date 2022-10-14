#ifndef C3230_ISR_HASH_TABLE
#define C3230_ISR_HASH_TABLE

#include "isr_linked_list.h"
#include <stddef.h>

#define ISR_HASH_TABLE_INIT_CAP (7)
#define ISR_HASH_TABLE_ALPHA (0.75)
#define ISR_HASH_TABLE_BETA (0.25)

/**
 * @brief (key, vale, hash_value) tuple which is stored in the hash_table
 *
 */
struct ISRHashTableEntity {
    void *key;
    void *value;
    size_t hash_value;
};

/**
 * @brief generic hash table
 * a generic hash table
 * hash_func is used to calculate the hash value
 * id_func is used to check whether two keys are identical
 */
struct ISRHashTable {
    size_t capacity;
    size_t size;
    size_t (*hash_func)(void *);
    int (*id_func)(void *, void *);
    struct ISRLinkedList **lists;
};

/**
 * @brief initialize a hash_table
 *
 * @param hash_func hash function
 * @param id_func identity function
 * @return struct ISRHashTable* the pointer to the hash table
 */
struct ISRHashTable *
isr_hash_table_new(size_t (*hash_func)(void *), int (*id_func)(void *, void *));

/**
 * @brief insert a (key, value) pair into the hash table.
 * If the size of hash table is larger than capacity * alpha, reallocate double capacity for the hash table.
 * If the key exists, replace it with new value
 * @note After the insertion, the ownership of key and value is passed to the hash table.
 *
 * @param table
 * @param key
 * @param value
 */
void isr_hash_table_insert(struct ISRHashTable *table, void *key, void *value);

/**
 * @brief remove one key from the hash table
 * if the size of hash table is smaller than capacity * beta, reallocate half capacity for the hash table.
 *
 * @param table
 * @param key
 * @return struct ISRHashTableEntity* the remove entity. NULL if entity not found.
 */
struct ISRHashTableEntity *isr_hash_table_remove(struct ISRHashTable *table, void *key);

/**
 * @brief remove one key from the hash table
 * if the size of hash table is smaller than capacity * beta, reallocate half capacity for the hash table.
 * If entity not found, do nothing.
 *
 * @param table
 * @param key
 */
void isr_hash_table_remove_dum(struct ISRHashTable *table, void *key);

/**
 * @brief search for a key in the hash table
 *
 * @param table
 * @param key
 * @return void* the (key, value, hash_value) tuple. NULL if not found
 */
void *isr_hash_table_find(struct ISRHashTable *table, void *key);

/**
 * @brief search for a key in the hash table
 *
 * @param table
 * @param key
 * @return int none-zero if find.
 */
int isr_hash_table_contains(struct ISRHashTable *table, void *key);

/**
 * @brief release the hash table
 *
 * @param table
 */
void isr_hash_table_free(struct ISRHashTable *table);

/**
 * @brief iterator the hash table. `p->value` should be the entity
 *
 */
#define ISRHashTableForEach(p, table)                                                              \
    for (size_t ISRHashTableIterator = 0; ISRHashTableIterator < (table)->capacity;                \
         ++ISRHashTableIterator)                                                                   \
    ISRLinkedListForEach(p, (table)->lists[ISRHashTableIterator])

#endif