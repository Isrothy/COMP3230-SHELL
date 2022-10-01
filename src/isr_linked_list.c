#include "../include/isr_linked_list.h"
#include <assert.h>
#include <stdlib.h>

struct ISRLinkedList *new_isr_linked_list() {
    struct ISRLinkedList *new_list = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
    new_list->sentinal = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    new_list->sentinal->value = NULL;
    new_list->sentinal->next = NULL;
    new_list->tail = new_list->sentinal;
    new_list->size = 0;
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    assert(list != NULL);
    list->tail->next = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    list->tail->next->value = value;
    list->tail->next->next = NULL;
    list->tail = list->tail->next;
    list->size += 1;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    return list->size == 0;
}

void isr_linked_list_free(struct ISRLinkedList *list, int deep) {
    struct ISRLinkedListNode *p = list->sentinal->next;
    while (p != NULL) {
        if (deep) {
            free(p->value);
        }
        struct ISRLinkedListNode *q = p->next;
        free(p);
        p = q;
    }
    free(list->sentinal);
    free(list);
}

// struct ISRLinkedList *insertHead(struct ISRLinkedList *head, void *value) {
//     struct ISRLinkedList *new_head = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
//     new_head->next = head;
//     new_head->value = value;
//     return new_head;
// }