#include "../include/isr_linked_list.h"
#include <assert.h>
#include <stdlib.h>

struct ISRLinkedList *new_isr_linked_list() {
    struct ISRLinkedList *new_list = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
    new_list->sentinal = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    // new_list->sentinal->next = NULL;
    new_list->sentinal->value = NULL;
    new_list->sentinal->next = NULL;
    new_list->tail = new_list->sentinal;
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    assert(list != NULL);
    list->tail->next = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    list->tail->next->value = value;
    list->tail->next->next = NULL;
    list->tail = list->tail->next;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    assert(list != NULL);
    return list->sentinal->next == NULL;
}


// struct ISRLinkedList *insertHead(struct ISRLinkedList *head, void *value) {
//     struct ISRLinkedList *new_head = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
//     new_head->next = head;
//     new_head->value = value;
//     return new_head;
// }