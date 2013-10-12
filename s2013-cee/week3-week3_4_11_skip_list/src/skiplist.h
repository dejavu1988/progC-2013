#ifndef SKIPLIST_H 
#define SKIPLIST_H 

typedef struct skiplistNode {
    int value;
    int levels;
    struct skiplistNode **next_pointers;
    struct skiplistNode **prev_pointers;
} SkipListNode;

typedef struct {
    int max_level;
    SkipListNode *header;
} SkipList;

SkipList* create_skip_list(int max_level);
SkipListNode* find(SkipList *skip_list, int value);
int decide_level(int max_level);
void insert_to_skip_list(SkipList *skip_list, int value);
void delete_from_skip_list(SkipList *skip_list, int value);
SkipList *reconstruct(SkipList *skip_list, int max_level);

#endif
