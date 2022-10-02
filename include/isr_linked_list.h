#ifndef C3230SHELL_ISR_LINKED_LIST
#define C3230SHELL_ISR_LINKED_LIST

#include <stddef.h>
struct ISRLinkedListNode {
    void *value;
    struct ISRLinkedListNode *next;
};

struct ISRLinkedList {
    size_t size;
    struct ISRLinkedListNode *sentinal;
    struct ISRLinkedListNode *tail;
};

struct ISRLinkedList *new_isr_linked_list();

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value);

int isr_linked_list_is_empty(struct ISRLinkedList *list);

struct ISRLinkedListNode *isr_linked_list_head(struct ISRLinkedList *list);

void isr_linked_list_free(struct ISRLinkedList *list, int deep);

#endif
