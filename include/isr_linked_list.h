#ifndef C3230SHELL_ISR_LINKED_LIST
#define C3230SHELL_ISR_LINKED_LIST

struct ISRLinkedListNode {
    void *value;
    struct ISRLinkedListNode *next;
};

struct ISRLinkedList {
    struct ISRLinkedListNode *sentinal;
    struct ISRLinkedListNode *tail;
};

struct ISRLinkedList *new_isr_linked_list();

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value);

int isr_linked_list_is_empty(struct ISRLinkedList *list);

#endif
