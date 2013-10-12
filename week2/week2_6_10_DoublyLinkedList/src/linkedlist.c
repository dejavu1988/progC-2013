#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

dList* create_node(int value){
    dList* dNode = malloc(sizeof(dList));
    dNode->val = value;
    dNode->next = NULL;
    dNode->prev = NULL;
    return dNode;
}

/* Exercise 6 */
dList* insert_element_d(dList *L, dList *p, int value) {
    dList* dNode = create_node(value);
    if(!L){
        return dNode;
    }else if(!p){
        dNode->next = L;
        L->prev = dNode;
        L = dNode;
        return L;
    }else{
        dNode->next = p->next;
        dNode->prev = p;
        if(p->next){
            p->next->prev = dNode;
        }
        p->next = dNode;
        return L;
    }
}

/* Exercise 7 */
int print_elements_d(dList *L) {
    dList* p = L;
    if(!L){
        return 1;
    }else{
        while(p){
            printf("%d\n", p->val);
            p = p->next;
        }
        return 0;
    }
}

/* Exercise 8 */
dList* delete_element_d(dList *L, dList *p) {
    dList* tmp = NULL;
    if(!L || !p){
        return L;
    }else{
        if(!(p->prev) && !(p->next)){
            free(p);
            return NULL;
        }else if(!(p->prev)){
            p->next->prev = NULL;
            tmp = p->next;
            free(p);
            return tmp;
        }else if(!(p->next)){
            p->prev->next = NULL;
            free(p);
            return L;
        }else{
            p->prev->next = p->next;
            p->next->prev = p->prev;
            free(p);
            return L;
        }
    }
}

/* Exercise 9 */
int order_list_d(dList *L) {
    dList *pos,*pos1,*p;
    
    if(!L) return 1;
    pos = L;
    while(pos){        
        int val = pos->val;
        pos1 = pos;
        p = pos1->next;
        while(p){
            if(p->val < val){
                pos1 = p;
            }
            p = p->next;
        }
        pos->val = pos1->val;
        pos1->val = val;
        pos = pos->next;
    }
	return 0;
}


/* Exercise 10 */
dList* merge_lists_d(dList *first, dList *second) {
    dList *pos1, *pos2, *list, *pos;
    pos1 = first;
    pos2 = second;
    list = pos = NULL;
    while(pos1 && pos2){
        if(pos1->val > pos2->val){
            list = insert_element_d(list, pos, pos2->val);
            pos2 = pos2->next;
            if(!pos)    pos = list;
            else    pos = pos->next;
        }else if(pos1->val < pos2->val){
            list = insert_element_d(list, pos, pos1->val); 
            pos1 = pos1->next;
            if(!pos)    pos = list;
            else    pos = pos->next;
        }else{
            list = insert_element_d(list, pos, pos1->val); 
            pos1 = pos1->next;
            pos2 = pos2->next;
            if(!pos)    pos = list;
            else    pos = pos->next;
        }
    }
    while(pos1){
        list = insert_element_d(list, pos, pos1->val); 
        pos1 = pos1->next;
        if(!pos)    pos = list;
        else    pos = pos->next;
    }
    while(pos2){
        list = insert_element_d(list, pos, pos2->val);
        pos2 = pos2->next;
        if(!pos)    pos = list;
        else    pos = pos->next;
    }
    
    return list;
}
