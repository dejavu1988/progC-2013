#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/shuffle.h"

// Helpers
int has_numbers(int count, int *array, int *original) {
	int found = 0;
	int i;
	for (i = 0; i < count; i++) {
		int j;
		for (j = 0; j < count; j++) {
			if (array[i] == original[j]) {
				found++;
				break;
			}
		}
	}
	return found == count;
}

int are_identical(int count, int *array, int *original) {
	int i;
	for (i = 0; i < count; i++) {
		if (array[i] != original[i]) {
			return 0;
		}
	}
	return 1;
}

void mark_positions(int count, int *array, int marks[][6]) {
	int i;
	for (i = 0; i < count; i++) {
		int val = array[i];
		marks[i][val]++;
	}
}

int is_approximately_uniform(int marks[][6], int count, int iterations, double epsilon) {
	int i;
	for (i = 0; i < count; i++) {
		int j;
		for (j = 0; j < count; j++) {
			double estimated_probability = (double)marks[i][j] / iterations;
			if (estimated_probability < (1.0/count) - epsilon || estimated_probability > (1.0/count) + epsilon) return 0;
		}
	}
	return 1;
}

// Tests
START_TEST(test_that_no_memory_errors_occur_when_shuffling)
{
	int array[5] = {1, -2, 3, -4, 5};
	int i;
	for (i = 0; i < 20; i++) {
		shuffle_ints(array, 5);
	}	
}
END_TEST

START_TEST(test_that_same_numbers_are_present_in_the_array_after_shuffling)
{
	int array[5] = {1, -2, 3, -4, 5};
	int original[5] = {1, -2, 3, -4, 5};
	int i;
	for (i = 0; i < 20; i++) {
		shuffle_ints(array, 5);
		fail_unless(has_numbers(5, array, original), "The same numbers should be found in the shuffled array");
	}
}
END_TEST

START_TEST(test_something_actually_changes_with_a_few_iterations)
{
	int array[5] = {1, -2, 3, -4, 5};
	int original[5] = {1, -2, 3, -4, 5};
	int i;
	int array_order_changed = 0;
	for (i = 0; i < 20; i++) {
		shuffle_ints(array, 5);
		if (!are_identical(5, array, original)) array_order_changed = 1;
	}
	fail_unless(array_order_changed, "Order of entries in the array should change somehow over a few iterations");
}
END_TEST

START_TEST(test_uniformity_of_shuffling)
{
	int marks[6][6] = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
	int iterations = 100000;
	int i;
	for (i = 0; i < iterations; i++) {
		int array[6] = {0, 1, 2, 3, 4, 5};
		shuffle_ints(array, 6);
		mark_positions(6, array, marks);
	}
	fail_unless(is_approximately_uniform(marks, 6, iterations, 0.005), "Number should be distributed approximately uniformly over many many iterations");
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

	tmc_register_test(s, test_that_no_memory_errors_occur_when_shuffling, "w2-4.0");
	tmc_register_test(s, test_that_same_numbers_are_present_in_the_array_after_shuffling, "w2-4.0");
	tmc_register_test(s, test_something_actually_changes_with_a_few_iterations, "w2-4.0");
	//tmc_register_test(s, test_uniformity_of_shuffling, "w2-4.0");
	TCase *tc_uniform = tcase_create("test_uniformity_of_shuffling");
	tcase_add_test(tc_uniform, test_uniformity_of_shuffling);
	tcase_set_timeout(tc_uniform, 10);
	tmc_set_tcase_points(tc_uniform, "test_uniformity_of_shuffling", "w2-4.0");
	suite_add_tcase(s, tc_uniform);
	return tmc_run_tests(argc, argv, s);
}
