#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

snodeType* create_node(int value){
    snodeType* sNode = (snodeType*) malloc(sizeof(snodeType));
    sNode->value = value;
    sNode->next = NULL;
    return sNode;
}

sList* create_sList(void) {
    sList *L = (sList*) malloc(sizeof(sList));
    L->count = 0;
    L->first = NULL;
    L->last = NULL;
	return L;
}


int insert_element_s(sList *L, snodeType *p, int value) {
    snodeType* node = create_node(value);
    if(!L) return 1;    
    if(!(L->count)){        
        L->first = node;
        L->last = node;
    }else if(!p){
        node->next = L->first;
        L->first = node;
    }else if(p == L->last){
        p->next = node;
        L->last = node;
    }else{
        node->next = p->next;
        p->next = node;
    }
    L->count ++;
	return 0;
}

int delete_element_s(sList *L, snodeType *p) {
    snodeType* tmp;
    if(!L || !L->count || (p == L->last)) return 1;
    if(!p){
        tmp = L->first;
        L->first = tmp->next;
        if(L->count == 1)
            L->last = NULL;
        free(tmp);
    }else{
        tmp = p->next;
        p->next = p->next->next;
        if(!p->next)
            L->last = p;
        free(tmp);
    }
    L->count --;
	return 0;
}

sList* merge_lists(sList *L1, sList *L2) {
    sList *list;
    snodeType *pos1, *pos2, *pos;
    pos1 = L1->first;
    pos2 = L2->first;
    list = create_sList();
    pos = NULL;
    while(pos1 && pos2){
        if(pos1->value > pos2->value){
            insert_element_s(list, pos, pos2->value);
            pos2 = pos2->next;
            if(!pos)    pos = list->first;
            else    pos = pos->next;
        }else if(pos1->value < pos2->value){
            insert_element_s(list, pos, pos1->value); 
            pos1 = pos1->next;
            if(!pos)    pos = list->first;
            else    pos = pos->next;
        }else{
            insert_element_s(list, pos, pos1->value); 
            pos1 = pos1->next;
            pos2 = pos2->next;
            if(!pos)    pos = list->first;
            else    pos = pos->next;
        }
    }
    while(pos1){
        insert_element_s(list, pos, pos1->value); 
        pos1 = pos1->next;
        if(!pos)    pos = list->first;
        else    pos = pos->next;
    }
    while(pos2){
        insert_element_s(list, pos, pos2->value);
        pos2 = pos2->next;
        if(!pos)    pos = list->first;
        else    pos = pos->next;
    }
	return list;
}