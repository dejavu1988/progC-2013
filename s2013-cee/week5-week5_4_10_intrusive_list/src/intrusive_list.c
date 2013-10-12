#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include "intrusive_list.h"

IntrusiveList* create_intrusive_list() {
    IntrusiveList* list = (IntrusiveList*) malloc(sizeof(IntrusiveList));
    list->header.prev = &(list->header);
    list->header.next = &(list->header);
    return list;
}

void add_node(IntrusiveList *list, IntrusiveListNode *node) {
    if(list && node){
        node->next = list->header.next;
        node->prev = &(list->header);
        list->header.next->prev = node;
        list->header.next = node;        
    }
}

void remove_node(IntrusiveList *list, IntrusiveListNode *node) {
    if(list && node){
        node->prev->next = node->next;
        node->next->prev = node->prev;        
        free(node);
    }
}

void add_int(IntrusiveList *list, int i) {
    WrapInt *intNode = (WrapInt*) malloc(sizeof(WrapInt));
    intNode->i = i;
    intNode->node.prev = NULL;
    intNode->node.next = NULL;
    add_node(list, &(intNode->node));
}

void remove_int(IntrusiveList *list, int r) {
    IntrusiveListNode *p = list->header.next;
    WrapInt *wi = NULL;
    while(p && p != &(list->header)){
        wi = container_of(p, WrapInt, node);
        if(wi->i == r){
            p->prev->next = p->next;
            p->next->prev = p->prev;
            free(wi);
            return;
        }
        p = p->next;
    }
}

void add_numeric(IntrusiveList *list, Numeric num, uint8_t type) {
    WrapNumeric *numNode = (WrapNumeric*) malloc(sizeof(WrapNumeric));
    numNode->type = type;
    if(type == INT_TYPE)
        numNode->num.i = num.i;
    else if(type == DOUBLE_TYPE)
        numNode->num.d = num.d;
    numNode->node.prev = NULL;
    numNode->node.next = NULL;
    add_node(list, &(numNode->node));
}

int sum_ints(IntrusiveList *list) {
    IntrusiveListNode *p = list->header.next;
    WrapNumeric *wn = NULL;
    int sum = 0;
    while(p && p != &(list->header)){
        wn = container_of(p, WrapNumeric, node);
        if(wn->type == INT_TYPE)
            sum += wn->num.i;
        p = p->next;
    }
    return sum;
}

double sum_doubles(IntrusiveList *list) {
    IntrusiveListNode *p = list->header.next;
    WrapNumeric *wn = NULL;
    double sum = 0.0;
    while(p && p != &(list->header)){
        wn = container_of(p, WrapNumeric, node);
        if(wn->type == DOUBLE_TYPE)
            sum += wn->num.d;
        p = p->next;
    }
    return sum;
}
