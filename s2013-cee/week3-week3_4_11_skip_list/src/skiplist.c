#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "skiplist.h"
#include "decide_level.h"

SkipListNode* create_node(int value, int level, int max_level) {
    SkipListNode *node = malloc(sizeof(SkipListNode)+2*max_level*sizeof(SkipListNode *));
    node->value = value;
    node->levels = level;
    node->prev_pointers = malloc(max_level*sizeof(SkipListNode *));
    node->next_pointers = malloc(max_level*sizeof(SkipListNode *));
    return node;
}
SkipList* create_skip_list(int max_level) {
    SkipList *list = malloc(sizeof(SkipList));
    list->max_level = max_level;
    list->header = create_node(INT_MAX,0,max_level);    
    for(int i=0;i<max_level;i++){
        list->header->prev_pointers[i] = NULL;
        list->header->next_pointers[i] = NULL;
    }
    return list;
}

SkipListNode* find(SkipList *skip_list, int value) {
    SkipListNode *p = skip_list->header;
    SkipListNode *q = NULL;   
    int k = skip_list->max_level; 
    for(int i=k-1;i>=0;i--){    
        while((q = p->next_pointers[i]) != NULL && q->value <= value){  
            if(q->value == value){
                return q;  
            }else{
                p = q;
            }
        }
    }  
    return NULL;
}

void insert_to_skip_list(SkipList *skip_list, int value) {
    SkipListNode *p = NULL;
    SkipListNode *q = NULL;  
    SkipListNode *candidate[skip_list->max_level];
    int k = skip_list->max_level;    
    p = skip_list->header;
    
    int r = skip_list->header->levels;
    
    for(int i=k-1;i>=0;i--){    
        while((q = p->next_pointers[i]) != NULL && q->value <= value){  
            if(q->value == value){
                return q;  
            }else{
                p = q;
            }
        }
        candidate[i] = p;
    }  
    
    int n = decide_level(k);
    if(n > r){
        for(int i=r; i < n; i++){    
            candidate[i] = skip_list->header;    
        }    
        skip_list->header->levels = n;
    }
    
    q = create_node(value,n,k);
    for(int i=0;i<n;i++){
        q->prev_pointers[i] = candidate[i];
        q->next_pointers[i] = candidate[i]->next_pointers[i];
        if(candidate[i]->next_pointers[i])
            candidate[i]->next_pointers[i]->prev_pointers[i] = q;
        candidate[i]->next_pointers[i] = q;
    }
}

void delete_from_skip_list(SkipList *skip_list, int value) {
    SkipListNode *p = NULL;
    SkipListNode *q = NULL;  
    SkipListNode *candidate[skip_list->max_level];
    //int k = skip_list->max_level;    
    p = skip_list->header;
    
    int r = skip_list->header->levels;
    
    for(int i=r-1;i>=0;i--){    
        while((q = p->next_pointers[i]) != NULL && q->value < value){  
            p = q;
        }
        candidate[i] = p;
    }  
    
    if(q && q->value == value){  
  
        for(int i=0; i<r; i++){ 
            if(candidate[i]->next_pointers[i]==q){  
                candidate[i]->next_pointers[i]=q->next_pointers[i];
                if(q->next_pointers[i])
                    q->next_pointers[i]->prev_pointers[i] = candidate[i];
            }    
        }     
        free(q);    
  
        for(int i=r-1; i >= 0; i--){      
            if(skip_list->header->next_pointers[i]==NULL){   
                skip_list->header->levels--;    
            }      
        }    
  
    }  
}

SkipList* reconstruct(SkipList *skip_list, int max_level) {
    SkipListNode *p = NULL;
    SkipListNode *new = NULL;      
    p = skip_list->header;
    new = create_skip_list(max_level);
    while(p->next_pointers[0]){
        insert_to_skip_list(new, p->next_pointers[0]->value);
        p = p->next_pointers[0];
    }
    return new;
}
