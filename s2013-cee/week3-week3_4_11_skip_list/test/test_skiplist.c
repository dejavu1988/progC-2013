#include <stdlib.h>
#include <check.h>
#include <stdint.h>
#include <limits.h>
#include "tmc-check.h"
#include "../src/skiplist.h"
#include <stdarg.h>

// Helpers
SkipList* CreateDummySkipListWithValues(int max_level, int count, ...) {
    va_list ap;
    SkipList *list = malloc(sizeof(SkipList));
    list->max_level = max_level;
    list->header = malloc(sizeof(SkipListNode));
    list->header->value = INT_MAX;
    list->header->levels = max_level;
    list->header->next_pointers = malloc(sizeof(SkipListNode*) * max_level);
    list->header->prev_pointers = malloc(sizeof(SkipListNode*) * max_level);
    va_start(ap, count);
    SkipListNode **level_tails = malloc(sizeof(SkipListNode*) * max_level);
    for (int i = 0; i < max_level; i++) {
        list->header->next_pointers[i] = NULL;
        list->header->prev_pointers[i] = NULL;
        level_tails[i] = list->header;
    }
    for (int i = 0; i < count; i++) {
        SkipListNode *node = malloc(sizeof(SkipListNode));
        int value = va_arg(ap, int);
        int level = va_arg(ap, int);
        node->value = value;
        node->levels = level;
        node->next_pointers = malloc(sizeof(SkipListNode*) * level);
        node->prev_pointers = malloc(sizeof(SkipListNode*) * level);
        for (int j = 0; j < level; j++) {
            node->next_pointers[j] = NULL;
            node->prev_pointers[j] = NULL;
        }
        for (int j = 0; j < level; j++) {
            node->prev_pointers[j] = level_tails[j];
            level_tails[j]->next_pointers[j] = node;
            level_tails[j] = node;
        }
    }
    va_end(ap);
    return list;
}

extern int __real_decide_level(int max_value);

int idx = 0;
int call_original = 0;
int array [3] = {1, 2, 3};

int decide_levelStub(int max_level) {
    int val = array[idx];
    idx = (idx + 1) % 3;
    return val;
}

int decide_level(int max_level) {
    if (call_original) {
        return __real_decide_level(max_level);
    }
    return decide_levelStub(max_level);
}

SkipListNode* GetNthNode(SkipListNode *starting_point, int level, int n) {
    SkipListNode *node = starting_point;
    for (int i = 0; i < n; i++) {
        node = node->next_pointers[level-1];
    }
    return node;
}

START_TEST(test_create_skip_list)
{
    SkipList *created = create_skip_list(5);
    fail_unless(created->max_level == 5, "Skip list should be created with the given max level");
    fail_unless(created->header->value == INT_MAX, "Skip lists headers should be initialized with a header node with INT_MAX as value");
}
END_TEST

START_TEST(test_find_from_skip_list)
{
    SkipList *list = CreateDummySkipListWithValues(3, 5,
            1, 2,
            3, 1,
            7, 1,
            10, 3,
            20, 2);
    SkipListNode *found = find(list, 1);
    fail_unless(found == list->header->next_pointers[0], "First element in the list should be found");
    found = find(list, 20);
    fail_unless(found == list->header->next_pointers[1]->next_pointers[1]->next_pointers[1], "Last element in the list should be found");
    found = find(list, 7);
    fail_unless(found == list->header->next_pointers[0]->next_pointers[0]->next_pointers[0], "An element from the middle of the list should be found");
    found = find(list, 11);
    fail_unless(found == NULL, "A non-existent value definately should not be found");
}
END_TEST

START_TEST(test_that_find_actually_searches_layers_above)
{
    SkipList *list = CreateDummySkipListWithValues(5, 5,
            2, 2,
            3, 1,
            6, 1,
            8, 3,
            11, 4);
    SkipListNode *second_last = GetNthNode(list->header, 1, 4);
    SkipListNode *second = GetNthNode(list->header, 1, 2);
    for (int i = 0; i < 3; i++) second->next_pointers[i] = NULL;
    for (int i = 0; i < second_last->levels; i++) second_last->next_pointers[i] = NULL;
    SkipListNode *found = find(list, 11);
    fail_unless(found == GetNthNode(list->header, 4, 1), "When disabling all other links to the last node except on the last layer, the node was not found when finding");
    found = find(list, 8);
    fail_unless(found == GetNthNode(list->header, 3, 1), "When disabling all other links to the third node except on the second last layer the node could not be found when calling find");
}
END_TEST

START_TEST(test_decide_level)
{
    call_original = 1;
    int levels [5] = {0};
    for (int i = 0; i < 1000000; i++) {
        int level = decide_level(5);
        levels[level-1]++;
    }
    
    fail_unless(levels[0] > 0 && levels[0] > levels[1], "There should be more of level ones than level two nodes, and at least one of them");
    fail_unless(levels[1] > 0 && levels[1] > levels[2], "There should be more of level 1 nodes than level 2 nodes, and at least one level 1 node");
    fail_unless(levels[2] > 0 && levels[2] > levels[3], "There should be more of level 2 nodes than level 3 nodes, and at least one level 2 node");
    fail_unless(levels[3] > 0, "There should be more level 3 nodes than level 4 nodes, and at least one level 3 node");
    fail_unless(levels[4] > 0, "There should be at least one level 4 node");
}
END_TEST

START_TEST(test_insertion_to_skip_list)
{
    call_original = 0;
    idx = 1;
    SkipList *list = CreateDummySkipListWithValues(3, 3, 
            1, 2, 
            3, 1, 
            4, 2);
    
    insert_to_skip_list(list, 2);
    
    fail_unless(GetNthNode(list->header, 1, 1)->value == 1, "Value of the first element should remain unchanged");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 2, "The second element should be the added element in the first layer when adding 2 to list 1 3 4");
    fail_unless(GetNthNode(list->header, 1, 3)->value == 3, "The third element should be the previous second element in the first layer  when adding 2 to list 1 3 4");
    fail_unless(GetNthNode(list->header, 1, 4)->value == 4, "The fourth element should be the previous third element in the first layer when adding 2 to list 1 3 4");

    // Test later layers
    fail_unless(GetNthNode(list->header, 2, 1)->value == 1, "The first value in the second layer should stay put when inserting 2 to layer 1 4");
    fail_unless(GetNthNode(list->header, 2, 2)->value == 2, "The second value in the second layer should be inserted when decide_level returns to when inserting 2 to layer 1 4");
    fail_unless(GetNthNode(list->header, 2, 3)->value == 4, "The third value in the second layer should be the previously second when inserting to layer 1 4");
}
END_TEST

START_TEST(test_insertion_to_beginning_of_skip_list)
{
    call_original = 0;
    idx = 2;
    SkipList *list = CreateDummySkipListWithValues(3, 3,
            2, 1,
            3, 1,
            4, 2);
    insert_to_skip_list(list, 1);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 1, "The first value should be the inserted in the first layer when inserting 1 to list 2 3 4");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 2, "The second value in the first layer should be the previous second when inserting 1 to list 2 3 4");
    fail_unless(GetNthNode(list->header, 1, 3)->value == 3, "The tail of the list shold stay unchanged in first layer when insertin 1 to 2 3 4");
    fail_unless(GetNthNode(list->header, 1, 4)->value == 4, "The tail of the list should stay unchanged in the first layer when inserting 1 to 2 3 4");
    fail_unless(GetNthNode(list->header, 1, 5) == NULL, "The tail of the list should stay unchanged in the first layer when inserting 1 to 2 3 4");
    
    // Later layer
    fail_unless(GetNthNode(list->header, 2, 1)->value == 1, "The first node in the second layer should be the inserted when inserting 1 to layer with 4");
    fail_unless(GetNthNode(list->header, 2, 2)->value == 4, "The second node in the second layer should be the previous first when inserting 1 to layer with 4");
    fail_unless(GetNthNode(list->header, 2, 3) == NULL, "There should be no third element in the second layer when inserting 1 to layer with 4");

    fail_unless(GetNthNode(list->header, 3, 1)->value == 1, "The first node in the third layer should be the inserted when inserting to empty layer 3");
    fail_unless(GetNthNode(list->header, 3, 2) == NULL, "The inserted node should be the only node in the third layer when inserting to empty layer 3");
}
END_TEST

START_TEST(test_insertion_to_tail_of_skip_list)
{
    call_original = 0;
    idx = 1;
    SkipList *list = CreateDummySkipListWithValues(3, 3,
            2, 1,
            4, 2,
            5, 2);
    insert_to_skip_list(list, 6);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 2, "The first value of the first layer should stay unchanged when inserting 6 to 2 4 5");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 4, "The second value of the first layer should stay unchanged when insrting 6 to 2 3 5");
    fail_unless(GetNthNode(list->header, 1, 3)->value == 5, "The third element should stay unchanged in the first layer when inserting 6 to 2 3 5");
    fail_unless(GetNthNode(list->header, 1, 4)->value == 6, "The last element should be the inserted in the first layer when inserting 6 to 2 3 5");
    
    // Later layer
    fail_unless(GetNthNode(list->header, 2, 1)->value == 4, "The first element in the second layer should be the previous first when inserting 6 to layer 4 5");
    fail_unless(GetNthNode(list->header, 2, 2)->value == 5, "The second element in the second layer should be the previous second when inserting 6 to layer 4 5");
    fail_unless(GetNthNode(list->header, 2, 3)->value == 6, "The third element in the second layer should be the inserted when inserting 6 to layer 4 5");
    fail_unless(GetNthNode(list->header, 2, 4) == NULL, "There should be only three elements in the second layer when inserting 6 to layer 4 5");

    fail_unless(GetNthNode(list->header, 3, 1) == NULL, "There should be no third layer when inserting node with level 2 to a list with empty third layer");
}
END_TEST

START_TEST(test_insertion_to_empty_skip_list)
{
    call_original = 0;
    idx = 0;
    SkipList *list = CreateDummySkipListWithValues(3, 0);
    insert_to_skip_list(list, 4);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 4, "A value should be inserted to the first layer of an empty skip list");

    // Layer layers
    fail_unless(GetNthNode(list->header, 2, 1) == NULL, "There should be no second layer when inserting a level 1 node to empty list");
    
    insert_to_skip_list(list, 1);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 1, "When 1 is inserted to  list with 4, the first value in the first layer should be 1");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 4, "When 1 is inserted to list with 4, the second value should be 4 in the first layer");

    // Later layers
    fail_unless(GetNthNode(list->header, 2, 1)->value == 1, "There first value in the second layer should be the inserted when inserting 1 to list with no second layer with a node of level 2");
    fail_unless(GetNthNode(list->header, 3, 1) == NULL, "There should be no third layer when inserting nodes with levels 1 and 2 to an empty list");
}
END_TEST

START_TEST(test_deletion_from_beginning_of_skip_list_in_two_layers)
{
    SkipList *list = CreateDummySkipListWithValues(3, 4,
            1, 2,
            2, 1,
            3, 2,
            4, 1);
    delete_from_skip_list(list, 1);
    fail_unless(list->header->next_pointers[0]->value == 2, "The first element should now be the second element in the lowest level linked list");
    fail_unless(list->header->next_pointers[1]->value == 3, "The first element in the second layer should now be the second in that layer");
    fail_unless(list->header->next_pointers[2] == NULL, "Third layer should still be empty");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->value == 3, "The second element in the first layer should be the previously third element");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->next_pointers[0]->value == 4, "The third element in the first layer should be the previously fourth element");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->next_pointers[0]->next_pointers[0] == NULL, "The first layer should not have any new elements in the end");
    fail_unless(list->header->next_pointers[1]->next_pointers[1] == NULL, "The second layer shouldn't have any items after the previously last one");
}
END_TEST

START_TEST(test_deletion_from_tail_of_skip_list)
{
    SkipList *list = CreateDummySkipListWithValues(3, 4,
            1, 2,
            2, 1,
            3, 2,
            4, 1);
    delete_from_skip_list(list, 4);

    // First layer correct
    fail_unless(list->header->next_pointers[0]->value == 1, "The first element in the first layer should still be in place");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->value == 2, "The second element in the first layer should still be in place");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->next_pointers[0]->value == 3, "The third element in the first layer should still be in place");
    fail_unless(list->header->next_pointers[0]->next_pointers[0]->next_pointers[0]->next_pointers[0] == NULL, "The link to the fourth element shouldn't exist anymore in the first layer");

    // Second layer correct
    fail_unless(list->header->next_pointers[1]->value == 1, "The first element in the second layer should still be in place");
    fail_unless(list->header->next_pointers[1]->next_pointers[1]->value == 3, "The second element in the second layer should still be in place");
    fail_unless(list->header->next_pointers[1]->next_pointers[1]->next_pointers[1] == NULL, "There should be no additional elements in the tail of the second layer");
    
    fail_unless(list->header->next_pointers[2] == NULL, "The third layer should still be empty");
}
END_TEST

START_TEST(test_deletion_from_middle_of_skip_list)
{
    SkipList *list = CreateDummySkipListWithValues(3, 4,
            1, 2,
            2, 1,
            3, 2,
            4, 1);
    delete_from_skip_list(list, 2);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 1, "The first value should stay in its place in the first layer");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 3, "The second value should be the third after deleting the second in the first layer");
    fail_unless(GetNthNode(list->header, 1, 3)->value == 4, "The third element should stay in place in the first level");
    fail_unless(GetNthNode(list->header, 1, 4) == NULL, "The fourth element should be null after deleting one node");
    
    fail_unless(GetNthNode(list->header, 2, 1)-> value == 1, "The first second layer element should stay in place");
    fail_unless(GetNthNode(list->header, 2, 2)->value == 3, "The second second layer element should still be the second when no second layer elements where deleted");
    fail_unless(GetNthNode(list->header, 2, 3) == NULL, "There should still be only two second layer elements");
    fail_unless(GetNthNode(list->header, 3, 1) == NULL, "There should be no third layer when there wasn't one in the fisrt place");

    delete_from_skip_list(list, 3);
    fail_unless(GetNthNode(list->header, 1, 1)->value == 1, "The first element should still be in place after deleting both middle nodes in the first layer");
    fail_unless(GetNthNode(list->header, 1, 2)->value == 4, "The second element should be the last after everything in the middle has been deleted in the first layer");
    fail_unless(GetNthNode(list->header, 1, 3) == NULL, "There should ne only two elements left in the first layer after deleting two out of four nodes");
    fail_unless(GetNthNode(list->header, 2, 1)->value == 1, "The first element in the second layer should still be in the list");
    fail_unless(GetNthNode(list->header, 2, 2) == NULL, "The second second layer element should not magically reappear after another deletion, when it was already previously deleted");
    fail_unless(GetNthNode(list->header, 3, 1) == NULL, "The third layer should still be empty without any insertions");
}
END_TEST

START_TEST(test_reconstruct_skip_list)
{
    call_original = 1;
    SkipList *list = CreateDummySkipListWithValues(3, 5,
            1, 2,
            2, 3,
            3, 1,
            4, 2,
            5, 3);
    SkipList *new_list = reconstruct(list, 2);
    fail_unless(new_list->max_level == 2);
    new_list = reconstruct(list, 1);
    fail_unless(new_list->max_level == 1);
}
END_TEST

START_TEST(test_reconstruct_with_large_list)
{
    call_original = 1;
    SkipList *list = CreateDummySkipListWithValues(3, 0);
    for (int i = 0; i < 500; i++) {
       insert_to_skip_list(list, i); 
    }
    
    SkipList *new_list = reconstruct(list, 5);
    for (int i = 0; i < 5; i++) {
        fail_unless(new_list->header->next_pointers[i] != NULL, "There should be at least one value per layer when a 500 value list is reconstructed from 3 to 5 layers");
    }
}
END_TEST

int main(int argc, const char *argv[]) {
    Suite *s = suite_create("Tests");

    tmc_register_test(s, test_create_skip_list, "w3-4.0");
    tmc_register_test(s, test_find_from_skip_list, "w3-5.0");
    tmc_register_test(s, test_that_find_actually_searches_layers_above, "w3-5.0");
    tmc_register_test(s, test_decide_level, "w3-6.0");
    tmc_register_test(s, test_insertion_to_skip_list, "w3-7.0");
    tmc_register_test(s, test_insertion_to_beginning_of_skip_list, "w3-7.0");
    tmc_register_test(s, test_insertion_to_tail_of_skip_list, "w3-7.0");
    tmc_register_test(s, test_insertion_to_empty_skip_list, "w3-7.0");
    tmc_register_test(s, test_deletion_from_beginning_of_skip_list_in_two_layers, "w3-8.0");
    tmc_register_test(s, test_deletion_from_tail_of_skip_list, "w3-8.0");
    tmc_register_test(s, test_deletion_from_middle_of_skip_list, "w3-8.0");
    tmc_register_test(s, test_reconstruct_skip_list, "w3-9.0");
    tmc_register_test(s, test_reconstruct_with_large_list, "w3-9.0");

    return tmc_run_tests(argc, argv, s);
}
