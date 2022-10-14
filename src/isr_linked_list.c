#include "../include/isr_linked_list.h"
#include <stdlib.h>

struct ISRLinkedList *isr_linked_list_new() {
    struct ISRLinkedList *new_list = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
    new_list->sentinel = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    *(new_list->sentinel) = (struct ISRLinkedListNode){
        NULL,
        new_list->sentinel,
        new_list->sentinel,
    };
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    struct ISRLinkedListNode *p
        = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    *p = (struct ISRLinkedListNode){
        value,
        list->sentinel,
        list->sentinel->prev,
    };
    p->next->prev = p;
    p->prev->next = p;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    return list->sentinel->next == list->sentinel;
}

int isr_linked_list_is_tail(struct ISRLinkedList *list, struct ISRLinkedListNode *p) {
    return p->next == list->sentinel;
}

void *isr_linked_list_del(struct ISRLinkedListNode *p) {
    void *ret = p->value;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    free(p);
    return ret;
}

void isr_linked_list_free(struct ISRLinkedList *list, const int deep) {
    struct ISRLinkedListNode *p = list->sentinel->next;
    while (p != list->sentinel) {
        struct ISRLinkedListNode *q = p->next;
        void *val = isr_linked_list_del(p);
        if (deep && val != NULL) {
            free(val);
        }
        p = q;
    }
    free(list->sentinel);
    free(list);
}
