#include "../include/isr_linked_list.h"
#include <stdlib.h>


struct ISRLinkedList *insertHead(struct ISRLinkedList *head, void *value) {
    struct ISRLinkedList *new_head = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
    new_head->next = head;
    new_head->value = value;
    return new_head;
}