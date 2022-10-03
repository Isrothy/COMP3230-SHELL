#include "../include/isr_linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct ISRLinkedList *isr_linked_list_new() {
    struct ISRLinkedList *new_list = malloc(sizeof(struct ISRLinkedList));
    new_list->sentinal = malloc(sizeof(struct ISRLinkedListNode));
    *(new_list->sentinal) = (struct ISRLinkedListNode){
        NULL,
        new_list->sentinal,
        new_list->sentinal,
    };
    new_list->size = 0;
    return new_list;
}

void isr_linked_list_insert_tail(struct ISRLinkedList *list, void *value) {
    assert(list != NULL);
    struct ISRLinkedListNode *p = malloc(sizeof(struct ISRLinkedListNode));
    *p = (struct ISRLinkedListNode){
        value,
        list->sentinal,
        list->sentinal->prev,
    };
    p->next->prev = p;
    p->prev->next = p;
    list->size += 1;
}

int isr_linked_list_is_empty(struct ISRLinkedList *list) {
    return list->size == 0;
}

void *isr_linked_list_del(struct ISRLinkedList *list, struct ISRLinkedListNode *p) {
    void *ret = p->value;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    --list->size;
    free(p);
    return ret;
}

void isr_linked_list_free(struct ISRLinkedList *list, int deep) {
    struct ISRLinkedListNode *p = list->sentinal->next;
    while (p != list->sentinal) {
        struct ISRLinkedListNode *q = p->next;
        void *val = isr_linked_list_del(list, p);
        if (deep) {
            free(val);
        }
        p = q;
    }
    free(list->sentinal);
    free(list);
}