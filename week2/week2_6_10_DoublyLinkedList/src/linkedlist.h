#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node {
	int val;
	struct node *next;
	struct node *prev;
} dList;

dList* insert_element_d(dList *L, dList *p, int value);
int print_elements_d(dList *L);
dList* delete_element_d(dList *L, dList *p);
int order_list_d(dList *L);
dList* merge_lists_d(dList *first, dList *second);

#endif /* LINKEDLIST_H */
