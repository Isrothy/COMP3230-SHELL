#ifndef C3230SHELL_ISR_LINKED_LIST
#define C3230SHELL_ISR_LINKED_LIST

struct ISRLinkedList {
    void *value;
    struct ISRLinkedList *next;
};

struct ISRLinkedList *insertHead(struct ISRLinkedList *head, void *value);

#endif
