#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/mathfuncs.h"

START_TEST(save_sum_sets_value_correctly_for_z)
{
	int* x = malloc(sizeof(int));
	int* y = malloc(sizeof(int));
	int* z = malloc(sizeof(int));


	*x = 1;
	*y = 2;
	save_sum(x, y, z);
	fail_unless(*z == 3, "Z should be 3 when a = 1 and y = 2");

	*x = 5;
	*y = 5;
	save_sum(x, y, z);
	fail_unless(*z == 10, "Z should be 10 when a = 5 and y = 5");

	*x = -20;
	*y = 1;
	save_sum(x, y, z);
	fail_unless(*z == -19, "Z should be -19 when a = -20 and y = 1");
}
END_TEST

START_TEST(average_works_for_one_number)
{
	double* x = malloc(sizeof(double));
	*x = 1.0;
	fail_unless(average(x, 1) == 1.0);
}
END_TEST

START_TEST(average_works_for_two_positive_numbers)
{
	double array[2];
	array[0] = 3;
	array[1] = 5;
	fail_unless(average(array, 2) == 4.0);
}
END_TEST

START_TEST(average_works_when_it_has_negative_arguments)
{
	double array[2];
	array[0] = -3;
	array[1] = 3;
	fail_unless(average(array, 2) == 0.0);
}
END_TEST

START_TEST(average_works_with_four_arguments)
{
	double array[4];
	array[0] = 10;
	array[1] = 11;
	array[2] = 19;
	array[3] = 19;
	double epsilon = 0.001;
	fail_unless(average(array, 4) < 14.75 + epsilon && average(array, 4) > 14.75 - epsilon, "Average should work with array 10 11 19 19 by being about 14.75");
}
END_TEST

START_TEST(subsum_does_insert_a_two_in_the_array)
{
	int array[5];
	array[0] = 1;
	array[1] = 1;
	array[2] = 1;
	array[3] = 1;
	array[4] = 1;
	substitute_and_sum(array, 5);
	short *short_array = (short*)array;
	fail_unless(short_array[4] == 2);
}
END_TEST

START_TEST(subsum_inserts_to_later_middle_index_when_array_of_even_size)
{
	int array[4];
	array[0] = 1;
	array[1] = 1;
	array[2] = 1;
	array[3] = 1;
	substitute_and_sum(array, 4);
	short *short_array = (short*)array;
	fail_unless(short_array[4] == 2);
}
END_TEST

START_TEST(subsum_should_return_two_when_given_an_array_with_only_zeros)
{
	int array[4];
	int i;
	for (i = 0; i < 4; i++) array[i] = 0;
	int val = substitute_and_sum(array, 4);
	fail_unless(val == 2);
}
END_TEST

START_TEST(subsum_should_work_with_more_complex_example)
{
	short short_array [10];
	int i;
	for (i = 0; i < 10; i++) short_array[i] = 0;
	short_array[0] = 10;
	short_array[7] = 97;
	short_array[8] = 110;
	int val = substitute_and_sum((int*)short_array, 5);
	fail_unless(val == 10 + 97 + 110 + 2);
}
END_TEST

START_TEST(subsum_should_work_with_another_more_complex_example)
{
	short short_array [10];
	int i;
	for (i = 0; i < 10; i++) short_array[i] = 0;
	short_array[0] = 10;
	short_array[1] = -99;
	short_array[7] = 97;
	short_array[8] = 110;
	int val = substitute_and_sum((int*)short_array, 5);
	fail_unless(val == 10 + 97 + 110 + 2 - 99);
}
END_TEST

START_TEST(test_subsum_with_hidden_examples)
{
FILE *fp = fopen("test/HiddenTestData2.txt", "r");
if (fp) {
	// If file exists, i.e., on server
	int randomDataRows = 100;
	int i;
	for (i = 0; i < randomDataRows; i++) {
		// Row contains given fraction, addition result, subtraction result, multiplication result, division result
		char row [200];
		char array_str [100];
		fgets(row, 200, fp);

		char *pch;
		pch = strtok(row, " \n");
		int array_length, expected_result;
		array_length = atoi(pch);
		int array [array_length];
		pch = strtok(NULL, " \n");
		int i = 0;
		while (pch != NULL && i < array_length) {
			int num;
			num = atoi(pch);
			array[i] = num;
			strcpy(array_str, pch); // For tooltip
			strcpy(array_str, " "); // For tooltip
			i++;
			pch = strtok(NULL, " \n");
		}
		expected_result = atoi(pch);
		int got_result = substitute_and_sum(array, array_length);

		char tooltip [200];
		strcpy(tooltip, "Failed with array: ");
		strcpy(tooltip, array_str);
		char additional_info [30];
		sprintf(additional_info, " Expected: %i, Got: %i", expected_result, got_result);
		strcpy(tooltip, additional_info);
		fail_unless(got_result == expected_result, tooltip);
		}
	}
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

	tmc_register_test(s, save_sum_sets_value_correctly_for_z, "w2-1.0");
	tmc_register_test(s, average_works_for_one_number, "w2-2.0");
	tmc_register_test(s, average_works_for_two_positive_numbers, "w2-2.0");
	tmc_register_test(s, average_works_when_it_has_negative_arguments, "w2-2.0");
	tmc_register_test(s, average_works_with_four_arguments, "w2-2.0");
	tmc_register_test(s, subsum_does_insert_a_two_in_the_array, "w2-3.0");
	tmc_register_test(s, subsum_inserts_to_later_middle_index_when_array_of_even_size, "w2-3.0");
	tmc_register_test(s, subsum_should_return_two_when_given_an_array_with_only_zeros, "w2-3.0");
	tmc_register_test(s, subsum_should_work_with_more_complex_example, "w2-3.0");
	tmc_register_test(s, subsum_should_work_with_another_more_complex_example, "w2-3.0");
	tmc_register_test(s, test_subsum_with_hidden_examples, "w2-3.0");

	return tmc_run_tests(argc, argv, s);
}