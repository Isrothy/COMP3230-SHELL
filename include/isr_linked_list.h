#ifndef C3230SHELL_ISR_LINKED_LIST
#define C3230SHELL_ISR_LINKED_LIST

#include <stddef.h>

/**
 * @brief a linked list node
 *
 */
struct ISRLinkedListNode {
    void *value;
    struct ISRLinkedListNode *next;
    struct ISRLinkedListNode *prev;
};

/**
 * @brief bie-direction generic linked list with a sentinal
 * sentinal->next is the head
 * sentinal->prev is the tail
 *
 */
struct ISRLinkedList {
    struct ISRLinkedListNode *sentinal;
};

/**
 * @brief get a new linked list.
 *
 * @return struct ISRLinkedList*
 */
struct ISRLinkedList *isr_linked_list_new();

/**
 * @brief insert value to the tail
 *
 * @param list
 * @param value
 */
void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value);

/**
 * @brief
 *
 * @param list
 * @return int none-zero if enpty
 */
int isr_linked_list_is_empty(struct ISRLinkedList *list);

/**
 * @brief
 *
 * @param list
 * @param p
 * @return int none-zero if is tail
 */
int isr_linked_list_is_tail(struct ISRLinkedList *list, struct ISRLinkedListNode *p);

/**
 * @brief remove node p from the list
 *
 * @param list
 * @param p
 * @return void* the value of p
 */
void *isr_linked_list_del(struct ISRLinkedListNode *p);

/**
 * @brief free a list. if deep then free value* of all nodes
 *
 * @param list
 * @param deep none-zero if deep free
 */
void isr_linked_list_free(struct ISRLinkedList *list, int deep);

/**
 * @brief iterator a linked list
 *
 */
#define ISRLinkedListForEach(p, l)                                                                 \
    for (struct ISRLinkedListNode * (p) = (l)->sentinal->next; (p) != (l)->sentinal;               \
         (p) = (p)->next)

#endif
