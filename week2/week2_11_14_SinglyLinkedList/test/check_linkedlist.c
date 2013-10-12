#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tmc-check.h"
#include "../src/linkedlist.h"

void freeList(sList *list) {
	snodeType *node = list->first;
	while (node != list->last && list->count != 0) {
		snodeType *next = node->next;
		free(node);
		node = next;
	}

	free(list);
}

sList* createDummyListWithElements(int count, ...) {
	va_list ap;
	int i;
	sList *list = malloc(sizeof(sList));
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
	va_start(ap, count);
	for (i = 0; i < count; i++) {
		list->count++;
		int value = va_arg(ap, int);
		snodeType *elem = malloc(sizeof(snodeType));
		elem->value = value;
		elem->next = NULL;
		if (list->last == NULL) {
			list->first = elem;
			list->last = elem;
			continue;
		}
		list->last->next = elem;
		list->last = elem;
	}
	va_end(ap);
	return list;
}

START_TEST(test_create_list)
{
	sList *list = createDummyListWithElements(0);
	fail_unless(list->count == 0, "The created list should contain the field count with a value of 0");
	fail_unless(list->first == NULL, "The created list should have a NULL first element");
	fail_unless(list->last == NULL, "The created list should have a NULL last element");
}
END_TEST

/* Insertion tests */
START_TEST(test_insertion_to_tail_of_single_element_list)
{
	sList *list = createDummyListWithElements(1, 1);

	insert_element_s(list, list->first, 99);
	fail_unless(list->count == 2, "Count of the list shold've increased by one when adding one new element to a single elemented list");
	fail_unless(list->first->value == 1, "First element in the list should remain to have value 1");
	fail_unless(list->first->next->value == 99, "Element to the right of the first element should have the inserted value 99");
	fail_unless(list->first->next == list->last, "Element to the right of the first element should be the last element after insertion");

	freeList(list);
}
END_TEST

START_TEST(test_insertion_to_single_element_list_with_null_position)
{
	sList *list = createDummyListWithElements(1, 1);

	insert_element_s(list, NULL, 99);
	fail_unless(list->count == 2, "Count of the list shold've increased by one when adding one new element to a single elemented list");
	fail_unless(list->first->value == 99, "First element in the list should now have inserted value 99");
	fail_unless(list->first->next->value == 1, "Element to the right of the first element should have the previous value 1");
	fail_unless(list->first->next == list->last, "Element to the right of the first element should be the last element after insertion");

	freeList(list);
}
END_TEST

START_TEST(test_insertion_to_empty_list)
{
	sList *list = createDummyListWithElements(0);

	insert_element_s(list, NULL, 99);

	fail_unless(list->count == 1, "There should be one element in the list after adding an element to an empty list");
	fail_unless(list->first == list->last, "First and last pointers should both point in the same node");
	fail_unless(list->first->value == 99, "Value of the first and last node should be the added 99");

	free(list);
}
END_TEST

START_TEST(test_insertion_to_head_of_two_elemented_list)
{
	sList *list = createDummyListWithElements(2, 1, 2);

	insert_element_s(list, NULL, 99);

	fail_unless(list->count == 3, "List should contain 3 elements when one additional element is added");
	fail_unless(list->first->value == 99, "The first element did not contain the added value with position NULL");
	fail_unless(list->first->next->value == 1 && list->last->value == 2, "The other two elements should remain in place");

	freeList(list);
}
END_TEST

START_TEST(test_insertion_to_middle_of_two_elemented_list)
{
	sList *list = createDummyListWithElements(2, 1, 2);

	insert_element_s(list, list->first, 99);

	fail_unless(list->count == 3, "List should contain 3 elements when one additional element is added");
	fail_unless(list->first->next->value == 99, "Second element should be one with the added value");
	fail_unless(list->first->value == 1 && list->last->value == 2, "The other two values should stay in place as first and last element");

	freeList(list);	
}
END_TEST

START_TEST(test_insertion_to_tail_of_two_elemented_list)
{
	sList *list = createDummyListWithElements(2, 1, 2);

	insert_element_s(list, list->last, 3);

	fail_unless(list->count == 3, "List should contain 3 elements when one additional element is added");
	fail_unless(list->last->value == 3, "The last element in the list should contain the added value 3");
	fail_unless(list->first->value == 1 && list->first->next->value == 2, "The other two values should stay in place as the two first elements");

	freeList(list);
}
END_TEST

/* Deletion */
START_TEST(test_deletion_for_empty_list)
{
	sList *list = createDummyListWithElements(0);

	fail_unless(delete_element_s(list, NULL) == 1, "Error code 1 should be returned when deleting from an empty list");

	free(list);
}
END_TEST

START_TEST(deletion_from_null_list_should_not_segfault)
{
	delete_element_s(NULL, NULL);
}
END_TEST

START_TEST(test_deletion_for_single_elemented_list)
{
	sList *list = createDummyListWithElements(1, 1);

	int status = delete_element_s(list, list->first);
	fail_unless(status == 1, "Status of deletion should be unsuccessful (1) when deleting with the only node in a list as position parameter");

	freeList(list);
}
END_TEST

START_TEST(test_deletion_with_null_position_in_two_elemented_list)
{

	sList *list = createDummyListWithElements(2, 1, 2);

	delete_element_s(list, NULL);
	fail_unless(list->count == 1, "There should be only one element remaining in the list when deleting one of two nodes");
	fail_unless(list->first->value == 2 && list->last->value == 2, "The value remaining in the list should be the value of the second element when deleting with null position");

	freeList(list);
}
END_TEST

START_TEST(test_deletion_of_second_element_from_two_elemented_list)
{
	sList *list = createDummyListWithElements(2, 1, 2);

	delete_element_s(list, list->first);
	fail_unless(list->count == 1, "There should be only one element remaining in the list when deleting one of two nodes");
	fail_unless(list->first->value == 1 && list->last->value == 1, "The only remaining element should be the first element when deleting the second element in a list with two elements");

	freeList(list);
}
END_TEST

START_TEST(test_merge_lists_with_two_empty_lists)
{
	sList *first = createDummyListWithElements(0);
	sList *second = createDummyListWithElements(0);

	sList *merged = merge_lists(first, second);

	fail_unless(merged->count == 0, "The merged list should contain 0 elements");
	fail_unless(merged->first == NULL && merged->last == NULL, "First and last elements should be null");

	freeList(first);
	freeList(second);
	freeList(merged);
}
END_TEST

START_TEST(test_merge_lists_with_one_elemented_list_and_empty_list)
{
	sList *first = createDummyListWithElements(1, 1);

	sList *second = createDummyListWithElements(0);

	sList *merged1 = merge_lists(first, second);
	sList *merged2 = merge_lists(second, first);
	fail_unless(merged1->count == 1, "The merged list should contain one value");
	fail_unless(merged1->first->value == 1 && merged1->last->value == 1, "The only value in the list should be the one found in the other list");	
	fail_unless(merged2->count == 1, "The merged list should contain one value");
	fail_unless(merged2->first->value == 1 && merged2->last->value == 1, "The only value in the list should be the one found in the other list");	

	freeList(first);
	freeList(second);
	freeList(merged1);
	freeList(merged2);
}
END_TEST

START_TEST(test_merge_with_two_elemented_identical_lists)
{
	sList *first = createDummyListWithElements(2, 1, 2);
	sList *second = createDummyListWithElements(2, 1, 2);

	sList *merged = merge_lists(first, second);
	fail_unless(merged->count == 2, "No duplicate values should be put into the merged list");
	fail_unless(merged->first->value == 1 && merged->last->value == 2, "The values should be in the correct order");

	fail_unless(first->first->value == 1 && first->first->next->value == 2, "The first parameter list should stay intact");

	freeList(first);
	freeList(second);
	freeList(merged);
}
END_TEST

START_TEST(test_merge_with_two_elemented_lists_containing_alternating_values_of_order)
{
	sList *first = createDummyListWithElements(2, 1, 3);
	sList *second = createDummyListWithElements(2, 2, 4);

	sList *merged = merge_lists(first, second);
	fail_unless(merged->count == 4, "All values should be in the merged list when there are no duplicates");
	fail_unless(merged->first->value == 1, "First value should be the smallest value in both lists");
	fail_unless(merged->first->next->value == 2, "The second value should be the second smallest value in both lists");
	fail_unless(merged->first->next->next->value == 3, "The third value should be the third smallest value in both lists");
	fail_unless(merged->last->value == 4, "The last value in a list should be the largest value in both lists");

	fail_unless(first->first->value == 1 && first->last->value == 3, "The first merged list should stay intact");
	fail_unless(second->first->value == 2 && second->last->value == 4, "The second merged list should stay intact");

	freeList(first);
	freeList(second);
	freeList(merged);
}
END_TEST

// Suite setup
Suite* linkedlist_suite(void) {
	Suite *s = suite_create("LinkedList");

	tmc_register_test(s, test_create_list, "w2-11.0");

	/* Insertion tests */
	tmc_register_test(s, test_insertion_to_tail_of_single_element_list, "w2-12.0");
	tmc_register_test(s, test_insertion_to_single_element_list_with_null_position, "w2-12.0");
	tmc_register_test(s, test_insertion_to_empty_list, "w2-12.0");
	tmc_register_test(s, test_insertion_to_head_of_two_elemented_list, "w2-12.0");
	tmc_register_test(s, test_insertion_to_middle_of_two_elemented_list, "w2-12.0");
	tmc_register_test(s, test_insertion_to_tail_of_two_elemented_list, "w2-12.0");

	/* Deletion tests */
	tmc_register_test(s, test_deletion_for_empty_list, "w2-13.0");
	tmc_register_test(s, deletion_from_null_list_should_not_segfault, "w2-13.0");
	tmc_register_test(s, test_deletion_for_single_elemented_list, "w2-13.0");
	tmc_register_test(s, test_deletion_with_null_position_in_two_elemented_list, "w2-13.0");
	tmc_register_test(s, test_deletion_of_second_element_from_two_elemented_list, "w2-13.0");

	/* Merge tests */
	tmc_register_test(s, test_merge_lists_with_two_empty_lists, "w2-14.0");
	tmc_register_test(s, test_merge_lists_with_one_elemented_list_and_empty_list, "w2-14.0");
	tmc_register_test(s, test_merge_with_two_elemented_identical_lists, "w2-14.0");
	tmc_register_test(s, test_merge_with_two_elemented_lists_containing_alternating_values_of_order, "w2-14.0");

	return s;
}

// Running tests
int main(int argc, const char *argv[]) {
	Suite *s = linkedlist_suite();
	return tmc_run_tests(argc, argv, s);
}