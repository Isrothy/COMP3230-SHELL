#include "../include/isr_linked_list.h"
#include <stdlib.h>

struct ISRLinkedList *isr_linked_list_new() {
    struct ISRLinkedList *new_list = (struct ISRLinkedList *) malloc(sizeof(struct ISRLinkedList));
    new_list->sentinal = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    *(new_list->sentinal) = (struct ISRLinkedListNode){
        NULL,
        new_list->sentinal,
        new_list->sentinal,
    };
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    struct ISRLinkedListNode *p
        = (struct ISRLinkedListNode *) malloc(sizeof(struct ISRLinkedListNode));
    *p = (struct ISRLinkedListNode){
        value,
        list->sentinal,
        list->sentinal->prev,
    };
    p->next->prev = p;
    p->prev->next = p;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    return list->sentinal->next == list->sentinal;
}

int isr_linked_list_is_tail(struct ISRLinkedList *list, struct ISRLinkedListNode *p) {
    return p->next == list->sentinal;
}

void *isr_linked_list_del(struct ISRLinkedListNode *p) {
    void *ret = p->value;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    free(p);
    return ret;
}

void isr_linked_list_free(struct ISRLinkedList *list, const int deep) {
    struct ISRLinkedListNode *p = list->sentinal->next;
    while (p != list->sentinal) {
        struct ISRLinkedListNode *q = p->next;
        void *val = isr_linked_list_del(p);
        if (deep && val != NULL) {
            free(val);
        }
        p = q;
    }
    free(list->sentinal);
    free(list);
}
