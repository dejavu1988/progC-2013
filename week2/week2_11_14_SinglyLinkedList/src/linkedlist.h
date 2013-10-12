#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct snode {
	int value;
	struct snode *next;
} snodeType;

typedef struct hnode {
	int count;
	snodeType *first;
	snodeType *last;
} sList;

sList* create_sList(void);
int insert_element_s(sList *L, snodeType *p, int value);
int delete_element_s(sList *L, snodeType *p);
sList* merge_lists(sList *L1, sList *L2);

#endif /* LINKEDLIST_H */
