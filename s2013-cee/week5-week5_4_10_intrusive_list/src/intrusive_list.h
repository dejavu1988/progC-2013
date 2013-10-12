#ifndef INTRUSIVE_H
#define INTRUSIVE_H

#define container_of(ptr, type, member) (type*)((char*)(ptr) - offsetof(type, member))

#define INT_TYPE 0
#define DOUBLE_TYPE 1

typedef struct intrusiveListNode {
    struct intrusiveListNode *next;
    struct intrusiveListNode *prev;
} IntrusiveListNode;

typedef struct intrusiveList {
    IntrusiveListNode header;
} IntrusiveList;

typedef struct wrapInt {
    int i;
    IntrusiveListNode node;
} WrapInt;

typedef union numeric {
    int i;
    double d;
} Numeric;

typedef struct wrapNumeric {
    uint8_t type;
    Numeric num;
    IntrusiveListNode node;
} WrapNumeric;

IntrusiveList* create_intrusive_list();
void add_node(IntrusiveList *list, IntrusiveListNode *node);
void remove_node(IntrusiveList *list, IntrusiveListNode *node);
void add_int(IntrusiveList *list, int i);
void remove_int(IntrusiveList *list, int r);
int get_first_int(IntrusiveList *list);
int get_last_int(IntrusiveList *list);

void add_numeric(IntrusiveList *list, Numeric num, uint8_t type);
int sum_ints(IntrusiveList *list);
double sum_doubles(IntrusiveList *list);

#endif
