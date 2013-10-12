#include <check.h>
#include "tmc-check.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include "../src/intrusive_list.h"

IntrusiveList* create_dummy_list() {
    IntrusiveList *list = malloc(sizeof(IntrusiveList));
    list->header.next = &list->header;
    list->header.prev = &list->header;
    return list;
}

IntrusiveList* create_dummy_list_with_n_nodes(int n) {
    IntrusiveList *list = create_dummy_list();
    for (int i = 0; i < n; i++) {
        IntrusiveListNode *node = malloc(sizeof(IntrusiveListNode));
        node->prev = &list->header;
        node->next = list->header.next;
        list->header.next->prev = node;
        list->header.next = node;
    }
    return list;
}

IntrusiveList* create_dummy_int_list_with_values(int count, ...) {
    va_list ap;

    IntrusiveList *list = create_dummy_list();
    IntrusiveListNode *node = &list->header;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        WrapInt *wi = malloc(sizeof(WrapInt));
        wi->node.prev = node;
        wi->node.next = &list->header;
        node->next = &wi->node;

        int i = va_arg(ap, int);
        wi->i = i;
        node = node->next;
    }
    node->next = &list->header;
    list->header.prev = node;
    va_end(ap);
    return list;
}

IntrusiveList* create_dummy_numeric_list_with_values(int count, ...) {
    va_list ap;

    IntrusiveList *list = create_dummy_list();
    IntrusiveListNode *node = &list->header;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        WrapNumeric *wn = malloc(sizeof(WrapNumeric));
        wn->node.prev = node;
        wn->node.next = &list->header;
        node->next = &wn->node;

        int type = va_arg(ap, int);
        wn->type = type;
        if (type == INT_TYPE) {
            int i = va_arg(ap, int);
            wn->num.i = i;
        } else { // DOUBLE_TYPE
            double d = va_arg(ap, double);
            wn->num.d = d;
        }
        node = node->next;
    }
    node->next = &list->header;
    list->header.prev = node;
    va_end(ap);
    return list;
}

// Test create intrusive list
START_TEST(test_create_intrusive_list)
{
    IntrusiveList *list = create_intrusive_list();
    fail_unless(list->header.prev == &list->header, "list->header.prev == &amp;list->header");
    fail_unless(list->header.next == &list->header, "list->header.next == &amp;list->header");
}
END_TEST

// Test add_node
START_TEST(test_add_node_to_empty_list)
{
    IntrusiveList *list = create_dummy_list();
    IntrusiveListNode *node = malloc(sizeof(IntrusiveListNode));
    add_node(list, node);
    fail_unless(list->header.next != &list->header, "list->header.next != &amp;list->header");
    fail_unless(list->header.prev != &list->header, "list->header.prev != &amp;list->header");
    fail_unless(list->header.next->next == &list->header, "list->header.next->next == &amp;list->header");
    fail_unless(list->header.prev->prev == &list->header, "list->header.prev->prev == &amp;list->header");
}
END_TEST

START_TEST(test_add_node_to_list_with_one_node)
{
    IntrusiveList *list = create_dummy_list_with_n_nodes(1);
    IntrusiveListNode *node = malloc(sizeof(IntrusiveListNode));
    add_node(list, node);
    fail_unless(list->header.next->next != &list->header, "list->header.next->next != &amp;list->header");
    fail_unless(list->header.next->next->next == &list->header, "list->header.next->next->next == &amp;list->header");
    fail_unless(list->header.prev->prev != &list->header, "list->header.prev->prev != &amp;list->header");
    fail_unless(list->header.prev->prev->prev == &list->header, "list->header.prev->prev->prev == &amp;list->header");
}
END_TEST

START_TEST(test_add_node_to_list_with_two_nodes)
{
    IntrusiveList *list = create_dummy_list_with_n_nodes(2);
    IntrusiveListNode *node = malloc(sizeof(IntrusiveListNode));
    add_node(list, node);
    fail_unless(list->header.next->next->next != &list->header, "list->header.next->next->next != &amp;list->header");
    fail_unless(list->header.next->next->next->next == &list->header, "list->header.next->next->next->next == &amp;list->header");
    fail_unless(list->header.prev->prev->prev != &list->header, "list->header.prev->prev->prev != &amp;list->header");
    fail_unless(list->header.prev->prev->prev->prev == &list->header, "list->header.prev->prev->prev->prev == &amp;list->header");
}
END_TEST

// Removes nodes test
START_TEST(test_remove_only_node_from_list)
{
    IntrusiveList *list = create_dummy_list_with_n_nodes(1);
    IntrusiveListNode *node = list->header.next;
    remove_node(list, node);
    fail_unless(list->header.next == &list->header, "list->header.next == &amp;list->header");
    fail_unless(list->header.prev == &list->header, "list->header.prev == &amp;list->header");
}
END_TEST

START_TEST(test_remove_first_node_in_two_elemented_list)
{
    IntrusiveList *list = create_dummy_list_with_n_nodes(2);
    IntrusiveListNode *node = list->header.next;
    remove_node(list, node);
    fail_unless(list->header.next != &list->header, "list->header.next != &amp;list->header");
    fail_unless(list->header.prev != &list->header, "list->header.prev != &amp;list->header");
    fail_unless(list->header.next->next == &list->header, "list->header.next->next == &amp;list->header");
    fail_unless(list->header.prev->prev == &list->header, "list->header.prev->prev == &amp;list->header");
}
END_TEST

START_TEST(test_remove_second_node_from_two_elemented_list)
{
    IntrusiveList *list = create_dummy_list_with_n_nodes(2);
    IntrusiveListNode *node = list->header.next->next;
    remove_node(list, node);
    fail_unless(list->header.next != &list->header, "list->header.next != &amp;list->header");
    fail_unless(list->header.prev != &list->header, "list->header.prev != &amp;list->header");
    fail_unless(list->header.next->next == &list->header, "list->header.next->next == &amp;list->header");
    fail_unless(list->header.prev->prev == &list->header, "list->header.prev->prev == &amp;list->header");
}
END_TEST

// Test add int
START_TEST(test_add_int_to_empty_list)
{
    IntrusiveList *list = create_dummy_list();
    WrapInt *wrapper = malloc(sizeof(WrapInt));
    wrapper->i = 10;
    add_node(list, &wrapper->node);

    IntrusiveListNode *first_node = list->header.next;
    WrapInt *container = container_of(first_node, WrapInt, node);
    fail_unless(container->i == 10, "Value of the first element after inserting 10 to empty list should be 10");
}
END_TEST

START_TEST(test_add_int_to_int_list_with_one_element)
{
    IntrusiveList *list = create_dummy_int_list_with_values(1, 9);
    add_int(list, 12);
    WrapInt *first = container_of(list->header.next, WrapInt, node);
    WrapInt *second = container_of(list->header.next->next, WrapInt, node);
    fail_unless(first->i == 12, "The first element should be the newly added 12, when adding to list with 9");
    fail_unless(second->i == 9, "The second element should be the old first element when adding 12 to list with 9");
}
END_TEST

START_TEST(test_add_int_to_int_list_with_two_elements)
{
    IntrusiveList *list = create_dummy_int_list_with_values(2, 9, 10);
    add_int(list, 8);
    WrapInt *first = container_of(list->header.next, WrapInt, node);
    WrapInt *second = container_of(list->header.next->next, WrapInt, node);
    WrapInt *third = container_of(list->header.next->next->next, WrapInt, node);
    fail_unless(first->i == 8, "The first element should be the newly added 8 when adding to list with 9 10");
    fail_unless(second->i == 9, "The second element should be the previous first element when adding 8 to list with 9 10");
    fail_unless(third->i == 10, "The third element should be the previous second element when adding 8 to list with 9 10");
}
END_TEST

// Test remove_int
START_TEST(test_removing_only_int)
{
    IntrusiveList *list = create_dummy_int_list_with_values(1, 2);
    remove_int(list, 2);
    fail_unless(list->header.next == &list->header, "There next pointer of the header should again point to the header");
    fail_unless(list->header.prev == &list->header, "The prev pointer of the header should again point to the header");
}
END_TEST

START_TEST(test_removing_from_two_element_int_list)
{
    IntrusiveList *list = create_dummy_int_list_with_values(2, 1, 2);
    remove_int(list, 1);
    WrapInt *wi = container_of(list->header.next, WrapInt, node);
    fail_unless(wi->i == 2, "When deleting the first node from list 1 2, the first value should be 2");
    fail_unless(list->header.next->next == &list->header, "When deleting the first node from list 1 2, there should be no second element");
    
    list = create_dummy_int_list_with_values(2, 1, 2);
    remove_int(list, 2);
    wi = container_of(list->header.next, WrapInt, node);
    fail_unless(wi->i == 1, "When deleting the second element from list 1 2, the first element should be 1");
    fail_unless(list->header.next->next == &list->header, "When deleting the second element from list 1 2, there should be no second element");
}
END_TEST

START_TEST(test_removing_int_not_found)
{
    IntrusiveList *list = create_dummy_int_list_with_values(2, 1, 2);
    remove_int(list, 3);
    WrapInt *first = container_of(list->header.next, WrapInt, node);
    WrapInt *second = container_of(list->header.next->next, WrapInt, node);
    fail_unless(first->i == 1 && second->i == 2, "Values should stay in the right place when trying to remove 3 from list with 1 2");
}
END_TEST

// Test add_numeric
START_TEST(test_add_numeric_to_empty_list)
{
    IntrusiveList *list = create_dummy_list();
    Numeric int_num;
    int_num.i = 10;
    add_numeric(list, int_num, INT_TYPE);
    
    WrapNumeric *wn = container_of(list->header.next, WrapNumeric, node);
    fail_unless(wn->num.i == 10, "The value of the added numeric should be 10 as integer, when 10 inserted as integer");
    fail_unless(wn->type == INT_TYPE, "The type of the inserted numeric should be set to INT_TYPE when added as INT_TYPE");
    
    list = create_dummy_list();
    Numeric double_num;
    double_num.d = 5.5;
    add_numeric(list, double_num, DOUBLE_TYPE);

    wn = container_of(list->header.next, WrapNumeric, node);
    fail_unless(wn->num.d == 5.5, "The value of the added numeric should be 5.5 as double when 5.5 inserted as double");
    fail_unless(wn->type == DOUBLE_TYPE, "The type of the inserted numeric should be set to DOUBLE_TYPE when added as DOUBLE_TYPE");
}
END_TEST

START_TEST(test_add_numeric_to_list_with_two_values)
{
    IntrusiveList *list = create_dummy_numeric_list_with_values(2, INT_TYPE, 1, DOUBLE_TYPE, 2.2);
    Numeric n;
    n.i = 10;
    add_numeric(list, n, INT_TYPE);

    WrapNumeric *first = container_of(list->header.next, WrapNumeric, node);
    WrapNumeric *second = container_of(list->header.next->next, WrapNumeric, node);
    WrapNumeric *third = container_of(list->header.next->next->next, WrapNumeric, node);
    
    fail_unless(first->num.i == 10, "The int value of the first element should be 10 when adding to list with 1 2.2");
    fail_unless(second->num.i == 1, "The int value of the second element should be 1 when adding 10 to list with 1 2.2");
    fail_unless(third->num.d == 2.2, "The double value of the third element should be 2.2 when adding 10 to list with 1 2.2");
}
END_TEST

// Test sum_ints
START_TEST(test_sum_ints_with_zero_elements)
{
    IntrusiveList *list = create_dummy_list();
    fail_unless(sum_ints(list) == 0);
}
END_TEST

START_TEST(test_sum_ints_with_one_element)
{
    IntrusiveList *list = create_dummy_numeric_list_with_values(1, INT_TYPE, 10);
    int sum = sum_ints(list);
    fail_unless(sum == 10, "Summing ints in list with only int value should return the value of the numeric");
}
END_TEST

START_TEST(test_sum_ints_with_two_elements)
{
    IntrusiveList *list = create_dummy_numeric_list_with_values(2, INT_TYPE, 3, INT_TYPE, 4);
    int sum = sum_ints(list);
    fail_unless(sum == 7, "Summing list with two int values 3 and 4 should equal 7");

    list = create_dummy_numeric_list_with_values(2, INT_TYPE, 3, DOUBLE_TYPE, 4.5);
    sum = sum_ints(list);
    fail_unless(sum == 3, "Summing list with int 3 and double 4.5 should equal 3");

    list = create_dummy_numeric_list_with_values(2, DOUBLE_TYPE, 3, DOUBLE_TYPE, 4);
    sum = sum_ints(list);
    fail_unless(sum == 0, "Summing list with only double values should equal 0");
}
END_TEST

// Test sum doubles
START_TEST(test_sum_doubles_with_zero_elements)
{
    IntrusiveList *list = create_dummy_list();
    fail_unless(sum_doubles(list) == 0);
}
END_TEST

START_TEST(test_sum_doubles_with_one_element)
{
    IntrusiveList *list = create_dummy_numeric_list_with_values(1, DOUBLE_TYPE, 2.5);
    fail_unless(sum_doubles(list) == 2.5);
}
END_TEST

START_TEST(test_sum_doubles_with_two_elements)
{
    IntrusiveList *list = create_dummy_numeric_list_with_values(2, INT_TYPE, 3, INT_TYPE, 4);
    double sum = sum_doubles(list);
    fail_unless(sum == 0, "Summing list with two int values 3 and 4 should equal 0");

    list = create_dummy_numeric_list_with_values(2, INT_TYPE, 3, DOUBLE_TYPE, 4.5);
    sum = sum_doubles(list);
    fail_unless(sum == 4.5, "Summing list with int 3 and double 4.5 should equal 4.5");

    list = create_dummy_numeric_list_with_values(2, DOUBLE_TYPE, 3.0, DOUBLE_TYPE, 4.0);
    sum = sum_doubles(list);
    fail_unless(sum == 7, "Summing list with double values 3 and 4 should equal 7");
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");
    
    tmc_register_test(s, test_create_intrusive_list, "w5-4.0");

    tmc_register_test(s, test_add_node_to_empty_list, "w5-5.0");
    tmc_register_test(s, test_add_node_to_list_with_one_node, "w5-5.0");
    tmc_register_test(s, test_add_node_to_list_with_two_nodes, "w5-5.0");

    tmc_register_test(s, test_remove_only_node_from_list, "w5-6.0");
    tmc_register_test(s, test_remove_first_node_in_two_elemented_list, "w5-6.0");
    tmc_register_test(s, test_remove_second_node_from_two_elemented_list, "w5-6.0");

    tmc_register_test(s, test_add_int_to_empty_list, "w5-7.0");
    tmc_register_test(s, test_add_int_to_int_list_with_one_element, "w5-7.0");
    tmc_register_test(s, test_add_int_to_int_list_with_two_elements, "w5-7.0");

    tmc_register_test(s, test_removing_only_int, "w5-8.0");
    tmc_register_test(s, test_removing_from_two_element_int_list, "w5-8.0");
    tmc_register_test(s, test_removing_int_not_found, "w5-8.0");

    tmc_register_test(s, test_add_numeric_to_empty_list, "w5-9.0");
    tmc_register_test(s, test_add_numeric_to_list_with_two_values, "w5-9.0");

    tmc_register_test(s, test_sum_ints_with_zero_elements, "w5-10.0");
    tmc_register_test(s, test_sum_ints_with_one_element, "w5-10.0");
    tmc_register_test(s, test_sum_ints_with_two_elements, "w5-10.0");
    tmc_register_test(s, test_sum_doubles_with_zero_elements, "w5-10.0");
    tmc_register_test(s, test_sum_doubles_with_one_element, "w5-10.0");
    tmc_register_test(s, test_sum_doubles_with_two_elements, "w5-10.0");

    return tmc_run_tests(argc, argv, s);
}
