#include "../include/isr_linked_list.h"
#include <assert.h>
#include <stdlib.h>

struct ISRLinkedList *isr_linked_list_new() {
    struct ISRLinkedList *new_list = malloc(sizeof(struct ISRLinkedList));
    new_list->sentinal = malloc(sizeof(struct ISRLinkedListNode));
    new_list->sentinal->value = NULL;
    new_list->sentinal->next = NULL;
    new_list->tail = new_list->sentinal;
    new_list->size = 0;
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    assert(list != NULL);
    list->tail->next = malloc(sizeof(struct ISRLinkedListNode));
    list->tail->next->value = value;
    list->tail->next->next = NULL;
    list->tail = list->tail->next;
    list->size += 1;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    return list->size == 0;
}

struct ISRLinkedListNode *isr_linked_list_head(struct ISRLinkedList *list) {
    return list->sentinal->next;
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