#ifndef C3230SHELL_ISR_LINKED_LIST
#define C3230SHELL_ISR_LINKED_LIST

#include <stddef.h>
struct ISRLinkedListNode {
    void *value;
    struct ISRLinkedListNode *next;
    struct ISRLinkedListNode *prev;
};

struct ISRLinkedList {
    size_t size;
    struct ISRLinkedListNode *sentinal;
};

struct ISRLinkedList *isr_linked_list_new();

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value);

int isr_linked_list_is_empty(struct ISRLinkedList *list);

void *isr_linked_list_del(struct ISRLinkedList *list, struct ISRLinkedListNode *p);

void isr_linked_list_free(struct ISRLinkedList *list, int deep);

#define ISRLinkedListForEach(p, l)                                                                 \
    for (struct ISRLinkedListNode *p = (l)->sentinal->next; p != (l)->sentinal; p = p->next)

#endif
