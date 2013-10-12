#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tmc-check.h"
#include "../src/linkedlist.h"

// TODO: fix these tests to not assume that some previously tested function is working because they have separate points, instead contruct the list manually via a helper method

// Helpers
dList* createDummyListWithValues(int count, ...) {
	va_list ap;
	int i;
	dList *list = NULL;
	dList *tail_node = NULL;
	va_start(ap, count);
	for (i = 0; i < count; i++) {
		int value = va_arg(ap, int);
		dList *new_node = malloc(sizeof(dList));
		new_node->val = value;
		new_node->next = NULL;
		new_node->prev = NULL;

		if (list == NULL) {
			list = new_node;
			tail_node = new_node;
			continue;
		}

		new_node->prev = tail_node;
		tail_node->next = new_node;
		tail_node = new_node;
 	}
	va_end(ap);
	return list;
}

void freeList(dList *list) {
	dList *next = list->next;
	free(list);
	while (next != NULL) {
		dList *current = next;
		next = next->next;
		free(current);
	}
}

// Insertion tests
START_TEST (test_insertion_empty_list)
{
	dList *list = insert_element_d(NULL, NULL, 1);
	int expected_value = 1;
	int val = list->val;
	fail_unless(val == expected_value, "The first value in the list after first insertion was not the inserted.");
	fail_unless(list->next == NULL, "The pointer to next node after addition of first value is not null.");
	fail_unless(list->prev == NULL, "The pointer to previous node after addition of first value is not null.");
	freeList(list);
}
END_TEST

START_TEST(test_insertion_to_null_position_of_list_with_one_element)
{
	dList *list = createDummyListWithValues(1, 2);
	list = insert_element_d(list, NULL, 1);
	fail_unless(list->val == 1, "Value of the first element should be that of the inserted");
	fail_unless(list->next->val == 2, "Value of the second element should be the one previously the tail of the list");
	fail_unless(list->next->next == NULL, "No more elements should magically appear to the end of the list");
	freeList(list);
}
END_TEST

START_TEST (test_insertion_to_beginning_of_list_with_one_element)
{
	dList *list = createDummyListWithValues(1, 1);
	list = insert_element_d(list, list, 2);
	int val = list->val;
	int expected_value = 1;
	fail_unless(val == expected_value, "The value of the first element should be the value of the first element.");
	fail_unless(list->next->val == 2, "The value of the next element should be the value of the newly added element.");
	fail_unless(list->next->prev->val == expected_value, "The value of the previous element of the next element should be that of the first element.");
	freeList(list);
}
END_TEST

START_TEST(test_insertion_to_null_position_of_list_with_two_elements)
{
	dList *list = createDummyListWithValues(2, 2, 3);
	list = insert_element_d(list, NULL, 1);
	fail_unless(list->val == 1, "Value of the first element should be that of the inserted");
	fail_unless(list->next->val == 2, "Value of the second element should be the one previously the first of the list");
	fail_unless(list->next->next->val == 3, "Value of the third element should be that of the previously second in the list");
	fail_unless(list->next->next->next == NULL, "No more elements should magically appear to the end of the list");
	freeList(list);
}
END_TEST

START_TEST (test_insertion_to_beginning_of_list_with_two_elements)
{
	dList *list = createDummyListWithValues(2, 1, 2);
	list = insert_element_d(list, list, 3);
	int val = list->next->val;
	int expected = 3;
	fail_unless(val == expected, "Value of the next of first should be the one inserted last.");
	fail_unless(list->next->next->val == 2, "The value of the next of the next should be the one inserted second.");
	freeList(list);
}
END_TEST

START_TEST (test_insertion_to_end_of_list_with_two_elements)
{
	dList *list = createDummyListWithValues(2, 1, 2);
	list = insert_element_d(list, list->next, 3);
	int val = list->next->next->val;
	int expected = 3;
	fail_unless(val == expected, "The value of last element should be what was inserted last.");
	fail_unless(list->next->next->next == NULL, "The next of the last element should be null.");
	fail_unless(list->next->next->prev->val == 2, "The value of the previous element of last should be the one inserted second.");
	freeList(list);
}
END_TEST

// Deletion tests
START_TEST (test_deletion_from_list_with_one_element)
{
	dList *list = createDummyListWithValues(1, 1);
	list = delete_element_d(list, list);
	fail_unless(list == NULL, "After deleting the only element, the list should be null.");
	free(list);
}
END_TEST

START_TEST (test_deletion_of_first_element_in_list_with_three_elements)
{
	dList *list = createDummyListWithValues(3, 1, 2, 3);
	list = delete_element_d(list, list);
	fail_unless(list->val == 2, "The value of the first element in the list should be that of the second element.");
	fail_unless(list->next->val == 3, "The value of the next of first should be that of the lastly inserted element (to the tail of the list).");
	fail_unless(list->next->next == NULL, "The next of the first element should be the last element in the list.");
	fail_unless(list->prev == NULL, "The first element in the list should not have a previous element.");
	freeList(list);
}
END_TEST

START_TEST (test_deletion_of_middle_element_from_list_with_three_elements)
{
	dList *list = createDummyListWithValues(3, 1, 2, 3);
	list = delete_element_d(list, list->next);
	fail_unless(list->val == 1, "The value of the first element in the list should be that of the first element.");
	fail_unless(list->next->val == 3, "The value of the next of first should be that of the lastly inserted element (to the tail of the list).");
	fail_unless(list->next->next == NULL, "The next of the first element should be the last element in the list.");
	fail_unless(list->prev == NULL, "The first element in the list should not have a previous element.");
	freeList(list);
}
END_TEST

START_TEST (test_deletion_of_tail_element_from_list_with_three_elements)
{
	dList *list = createDummyListWithValues(3, 1, 2, 3);
	list = delete_element_d(list, list->next->next);
	fail_unless(list->val == 1, "The value of the first element in the list should be that of the first element.");
	fail_unless(list->next->val == 2, "The value of the next of first should be that of the second inserted element (to the tail of the list).");
	fail_unless(list->next->next == NULL, "The next of the first element should be the last element in the list.");
	fail_unless(list->prev == NULL, "The first element in the list should not have a previous element.");
	freeList(list);
}
END_TEST

START_TEST (test_deletion_with_null_element_in_list_with_three_elements)
{
	dList *list = createDummyListWithValues(3, 1, 2, 3);
	list = delete_element_d(list, NULL);
	fail_unless(list->val == 1, "The first element should be the one inserted as first.");
	fail_unless(list->next->val == 2, "The second element should be the one inserted as second.");
	fail_unless(list->next->next->val == 3, "The third element should be the one inserted third.");
	freeList(list);
}
END_TEST

// Sorting tests
START_TEST (test_sorting_with_empty_list)
{
	int success = order_list_d(NULL);
}
END_TEST

START_TEST (test_sorting_with_list_of_one_element)
{
	dList *list = createDummyListWithValues(1, 1);
	order_list_d(list);
	fail_unless(list->val == 1, "The first element in the list should be the the one inserted to the original list.");
	fail_unless(list->next == NULL, "The element in the ordered list should be the only element.");
	fail_unless(list->prev == NULL, "The element in the ordered list should not have a previous element.");
	freeList(list);
}
END_TEST

START_TEST (test_sorting_with_list_of_two_elements_in_correct_order)
{
	dList *list = createDummyListWithValues(2, 1, 2);
	order_list_d(list);
	fail_unless(list->val == 1, "The first element should remain as the first element.");
	fail_unless(list->next->val == 2, "The second element should remain as the second element.");
	freeList(list);
}
END_TEST

START_TEST (test_sorting_with_list_of_two_elements_in_reverse_order)
{
	dList *list = createDummyListWithValues(2, 2, 1);
	order_list_d(list);
	fail_unless(list->val == 1, "The second element should now be the first element.");
	fail_unless(list->next->val == 2, "The first element should now the the second element.");
	fail_unless(list->prev == NULL, "The first element should no longer have a previous element.");
	fail_unless(list->next->prev->val == 1, "The prev of the second element should be the first element.");
	fail_unless(list->next->next == NULL, "The second element should not have a next element.");
	freeList(list);
}
END_TEST

// Merge tests
START_TEST (test_merge_with_two_empty_lists)
{
	dList *merged = merge_lists_d(NULL, NULL);
	fail_unless(merged == NULL, "The merged list should also be empty (NULL).");
	free(merged);
}
END_TEST

START_TEST (test_merge_of_one_elemented_list_with_empty_list)
{
	dList *singleElement = createDummyListWithValues(1, 1);
	dList *merged = merge_lists_d(NULL, singleElement);
	fail_unless(merged->val == 1, "The first element of the merged list should be that of the single-elemented list.");
	fail_unless(merged->next == NULL, "There shouldn't be a second element.");
	fail_unless(merged->prev == NULL, "There should not be a previous element for the head of the merged list.");
	fail_unless(singleElement->val == 1, "The first element of the single-elemeneted list should remain unchanged.");
	fail_unless(singleElement->next == NULL, "The first element of the single-elemeneted list should remain unchanged.");
	fail_unless(singleElement->prev == NULL, "The first element of the single-elemeneted list should remain unchanged.");
	freeList(singleElement);
	freeList(merged);
}
END_TEST

START_TEST (test_merge_of_two_one_elemented_lists_with_different_values)
{
	dList *first = createDummyListWithValues(1, 1);
	dList *second = createDummyListWithValues(1, 2);
	dList *merged = merge_lists_d(first, second);
	fail_unless(merged->val == 1, "First value in merged list is not the value of the first list in merge.");
	fail_unless(merged->next->val == 2, "The second value in merged list is not the value of the second list in merge.");
	fail_unless(first->val == 1, "The first list should stay unchanged.");
	fail_unless(first->next == NULL, "The first list should stay unchanged.");
	fail_unless(first->prev == NULL, "The first list should stay unchanged.");
	fail_unless(second->val == 2, "The second list should stay unchanged.");
	fail_unless(second->next == NULL, "The second list should stay unchanged.");
	fail_unless(second->prev == NULL, "The second list should stay unchanged.");
	freeList(first);
	freeList(second);
	freeList(merged);
}
END_TEST

START_TEST (test_merge_of_two_two_elemented_lists_with_one_duplicate_value)
{
	dList *first = createDummyListWithValues(2, 1, 2);
	dList *second = createDummyListWithValues(2, 2, 3);
	dList *merged = merge_lists_d(first, second);

	fail_unless(merged->val == 1, "The first value should be the min of both lists.");
	fail_unless(merged->next->val == 2, "The second value should be the second lowest value in both lists.");
	fail_unless(merged->next->next->val == 3, "The third value should be the maximum value in both lists, since the second greatest value has a duplicate.");
	fail_unless(first->next->val == 2, "The duplicate in first list should be unchanged.");
	fail_unless(second->val == 2, "The duplicate in the second list should be unchanged.");

	freeList(first);
	freeList(second);
	freeList(merged);
}
END_TEST

// Element printing tests
START_TEST (test_printing_with_empty_list)
{
	dList *list = NULL;
	freopen("mockoutput.txt", "w", stdout);
	int exit_code = print_elements_d(list);
	fclose(stdout);
	fail_unless(exit_code == 1);
	FILE *fp = fopen("mockoutput", "r");
	fail_if(fp, "There should be no output when the printed list is empty.");
}
END_TEST

START_TEST (test_printing_list_with_one_element)
{
	dList *list = createDummyListWithValues(1, 1);
	freopen("mockoutput.txt", "w", stdout);
	int exit_code = print_elements_d(list);
	fclose(stdout);

	fail_unless(exit_code == 0);

	FILE *fp = fopen("mockoutput.txt", "r");
	fail_unless(fgetc(fp) == '1', "The first character should be the value of the only element.");
	fail_unless(fgetc(fp) == '\n', "The second character should be a linebreak.");
	fail_unless(fgetc(fp) == EOF, "The third character should be EOF.");
	fclose(fp);

	fail_unless(list->val == 1 && list->next == NULL, "The printed list should stay unchanged");
	freeList(list);

}
END_TEST

START_TEST (test_printing_list_with_two_elements)
{
	dList *list = createDummyListWithValues(2, 1, 3);
	freopen("mockoutput.txt", "w", stdout);
	int exit_code = print_elements_d(list);
	fclose(stdout);

	fail_unless(exit_code == 0);

	FILE *fp = fopen("mockoutput.txt", "r");
	fail_unless(fgetc(fp) == '1', "The first character should be the value of the first element");
	fail_unless(fgetc(fp) == '\n', "The second character should be a newline");
	fail_unless(fgetc(fp) == '3', "The third character should be the second value in the list.");
	fail_unless(fgetc(fp) == '\n', "The fourth character should be a newline.");
	fail_unless(fgetc(fp) == EOF, "The fifth character should be the EOF.");
	fclose(fp);

	fail_unless(list->val == 1 && list->next->val == 3 && list->next->next == NULL, "The printed list should stay unchanged");
	freeList(list);
}
END_TEST

// Suite setup
Suite* linkedlist_suite(void) {
	Suite *s = suite_create("LinkedList");
	// Insertion tests
	tmc_register_test(s, test_insertion_empty_list, "w2-6.0");
	tmc_register_test(s, test_insertion_to_null_position_of_list_with_one_element, "w2-6.0");
	tmc_register_test(s, test_insertion_to_beginning_of_list_with_one_element, "w2-6.0");
	tmc_register_test(s, test_insertion_to_null_position_of_list_with_two_elements, "w2-6.0");
	tmc_register_test(s, test_insertion_to_beginning_of_list_with_two_elements, "w2-6.0");
	tmc_register_test(s, test_insertion_to_end_of_list_with_two_elements, "w2-6.0");

	// Element printing tests
	tmc_register_test(s, test_printing_with_empty_list, "w2-7.0");
	tmc_register_test(s, test_printing_list_with_one_element, "w2-7.0");
	tmc_register_test(s, test_printing_list_with_two_elements, "w2-7.0");

	// Deletion tests
	tmc_register_test(s, test_deletion_from_list_with_one_element, "w2-8.0");
	tmc_register_test(s, test_deletion_of_first_element_in_list_with_three_elements, "w2-8.0");
	tmc_register_test(s, test_deletion_of_middle_element_from_list_with_three_elements, "w2-8.0");
	tmc_register_test(s, test_deletion_of_tail_element_from_list_with_three_elements, "w2-8.0");
	tmc_register_test(s, test_deletion_with_null_element_in_list_with_three_elements, "w2-8.0");

	// Sorting tests
	tmc_register_test(s, test_sorting_with_empty_list, "w2-9.0");
	tmc_register_test(s, test_sorting_with_list_of_one_element, "w2-9.0");
	tmc_register_test(s, test_sorting_with_list_of_two_elements_in_correct_order, "w2-9.0");
	tmc_register_test(s, test_sorting_with_list_of_two_elements_in_reverse_order, "w2-9.0");

	// Merge tests
	tmc_register_test(s, test_merge_with_two_empty_lists, "w2-10.0");
	tmc_register_test(s, test_merge_of_one_elemented_list_with_empty_list, "w2-10.0");
	tmc_register_test(s, test_merge_of_two_one_elemented_lists_with_different_values, "w2-10.0");
	tmc_register_test(s, test_merge_of_two_two_elemented_lists_with_one_duplicate_value, "w2-10.0");

	return s;
}

// Running tests
int main(int argc, const char *argv[]) {
	Suite *suite = linkedlist_suite();
	return tmc_run_tests(argc, argv, suite);
}