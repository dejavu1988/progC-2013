#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/polynomial.h"

// Helpers
typedef enum {
	add=0,
	subtract=1,
	multiply=2,
	divide=3
} operation_type;

void run_hidden_tests(operation_type op_type, int (*operation) (int, int, int, int)) {
	FILE *fp = fopen("test/DataHiddenTest.txt", "r");
	if (fp) {
		// If file exists, i.e., on server
		int randomDataRows = 100;
		int i;
		for (i = 0; i < randomDataRows; i++) {
			// Row contains given fraction, addition result, subtraction result, multiplication result, division result
			int firstInputUpper = 0;
			int firstInputLower = 0;
			int secondInputUpper = 0;
			int secondInputLower = 0;
			int addUpper = 0;
			int addLower = 0;
			int subUpper = 0;
			int subLower = 0;
			int mulUpper = 0;
			int mulLower = 0;
			int divUpper = 0;
			int divLower = 0;

			fscanf(fp, "%i/%i %i/%i %i/%i %i/%i %i/%i %i/%i\n", &firstInputUpper, &firstInputLower, 
				&secondInputUpper, &secondInputLower,
				&addUpper, &addLower,
				&subUpper, &subLower,
				&mulUpper, &mulLower,
				&divUpper, &divLower);
			/*printf("%i/%i %i/%i %i/%i %i/%i %i/%i %i/%i\n", firstInputUpper, firstInputLower, 
				secondInputUpper, secondInputLower,
				addUpper, addLower,
				subUpper, subLower,
				mulUpper, mulLower,
				divUpper, divLower);// Print params for debugging*/ 

			freopen("mockoutput", "w", stdout);
			// add_fraction(firstInputUpper, firstInputLower, secondInputUpper, secondInputLower); // Abstract this out
			operation(firstInputUpper, firstInputLower, secondInputUpper, secondInputLower);
			fclose(stdout);
			FILE *mockoutputFp = fopen("mockoutput", "r");
			char mockstr [100];
			fgets(mockstr, 100, mockoutputFp);
			char result [50];

			if (op_type == add) {
				sprintf(result, "Result: %i/%i\n", addUpper, addLower); // Abstract this out
			} else if (op_type == subtract) {
				sprintf(result, "Result: %i/%i\n", subUpper, subLower); // Abstract this out
			} else if (op_type == multiply) {
				sprintf(result, "Result: %i/%i\n", mulUpper, mulLower); // Abstract this out
			} else if (op_type == divide) {
				sprintf(result, "Result: %i/%i\n", divUpper, divLower); // Abstract this out
			}
			
			char tooltip [100];
			strcpy(tooltip, "Expected: ");
			strcat(tooltip, result);
			strcat(tooltip, " got: ");
			strcat(tooltip, mockstr);
			strcat(tooltip, " with: ");
			char frac [50];
			sprintf(frac, "%i/%i + %i/%i", firstInputUpper, firstInputLower, secondInputUpper, secondInputLower);
			strcat(tooltip, frac);
			fail_unless(!strcmp(mockstr, result), tooltip);
			fclose(mockoutputFp);
			fclose(stdout);
		}
	}
}

// Addition
START_TEST(adding_fractions_with_same_denominator_works)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(1, 3, 1, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 2/3\n"), "1/3 + 1/3 should result in printing: Result: 2/3\\n");
	fclose(fp);
}
END_TEST


START_TEST(adding_fractions_with_another_input)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(19, 43, 80, 27);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3953/1161\n"), "19/43 + 80/27 should result in printing: Result: 2/3\\n");
	fclose(fp);

}
END_TEST

START_TEST(adding_fractions_causes_simplification)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(1, 3, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 1/1\n"), "1/3 + 2/3 should result in printing: Result: 1/1\\n");
	fclose(fp);
}
END_TEST

START_TEST(adding_fractions_with_zero_denominator_gives_error)
{
	fail_unless(add_fraction(1, 0, 1, 5) == 0, "Giving a zero denominator fraction as a parameter should result in a status code 0");
}
END_TEST

START_TEST(adding_one_fifth_to_one_tenth_should_produce_three_tenths)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(1, 5, 1, 10);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/10\n"), "1/5 + 1/10 should result in printing: Result: 3/10\\n");
	fclose(fp);
}
END_TEST

START_TEST(adding_two_negative_signed_fractions_works_with_first_argument_negatives)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(-1, 5, -1, 10);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -3/10\n"), "-1/5 + -1/10 should result in printing: Result: -3/10\\n");
	fclose(fp);
}
END_TEST

START_TEST(adding_two_negative_signed_fractions_with_both_argument_negatives)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(-1, -5, -1, -10);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/10\n"), "-1/-5 + -1/-10 should result in printing: Result: 3/10\\n");
	fclose(fp);
}
END_TEST

START_TEST(adding_a_positive_value_to_a_larger_negative_value_keeps_result_negative)
{
	freopen("mockoutput", "w", stdout);
	add_fraction(-5, 6, 1, 2);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -1/3\n"), "-5/6 + 1/2 should result in printing: Result: -1/3\\n");
	fclose(fp);
}
END_TEST

START_TEST(test_adding_fractions_with_hidden_data_when_on_server)
{
	run_hidden_tests(add, add_fraction);
}
END_TEST

// Subtraction
START_TEST(subtracting_fractions_with_same_denominator_works)
{
	freopen("mockoutput", "w", stdout);
	sub_fraction(2, 3, 1, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 1/3\n"), "2/3 - 1/3 should result in printing: Result: 1/3\\n");
	fclose(fp);
}
END_TEST

START_TEST(subtracting_fractions_causes_simplification)
{
	freopen("mockoutput", "w", stdout);
	sub_fraction(5, 3, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 1/1\n"), "5/3 - 2/3 should result in printing: Result: 1/1\\n");
	fclose(fp);
}
END_TEST

START_TEST(subtracting_fractions_with_zero_denominator_gives_error)
{
	fail_unless(sub_fraction(1, 0, 1, 5) == 0, "Passing a zero denominator polynomial as an argument to substraction should result in status code 0");
}
END_TEST

START_TEST(subtraction_with_second_negative_argument_works)
{
	freopen("mockoutput", "w", stdout);
	sub_fraction(5, 3, -2, 3);
	sub_fraction(5, 3, 2, -3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 7/3\n"), "5/3 - (-2/3) should result in printing: Result: 7/3\\n");
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 7/3\n"), "5/3 - (2/-3) should result in printing: Result: 7/3\\n");
	fclose(fp);	
}
END_TEST

START_TEST(subtraction_with_both_values_negative)
{
	freopen("mockoutput", "w", stdout);
	sub_fraction(-5, 3, -2, 3);
	sub_fraction(-5, -3, -2, -3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -1/1\n"), "-5/3 - (-2/3) should result in printing: Result: -1/1\\n");
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 1/1\n"), "-5/-3 - (-2/-3) should result in printing: Result: 1/1\\n");
	fclose(fp);	
}
END_TEST

START_TEST(test_subtraction_with_hidden_data_when_on_server)
{
	run_hidden_tests(subtract, sub_fraction);
}
END_TEST

// Multiplication
START_TEST(multiplication_should_produce_correct_result_when_not_simplifying)
{
	freopen("mockoutput", "w", stdout);
	mul_fraction(5, 3, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 10/9\n"), "5/3 * 2/3 should result in printing: Result: 10/9\\n");
	fclose(fp);	
}
END_TEST

START_TEST(multiplication_should_simplify_when_possible)
{
	freopen("mockoutput", "w", stdout);
	mul_fraction(5, 5, 3, 2);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "5/5 * 3/2 should result in printing: Result: 3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(multiplication_should_fail_when_there_is_a_zero_denominator)
{
	fail_unless(mul_fraction(1, 0, 1, 5) == 0, "Passing a zero denominator polynomial to multiplication should result in status code 0");
}
END_TEST

START_TEST(multiplication_with_one_negative_parameter_gives_negative_result)
{
	freopen("mockoutput", "w", stdout);
	mul_fraction(-5, 5, 3, 2);
	mul_fraction(-5, -5, 3, 2);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -3/2\n"), "-5/5 * 3/2 should result in printing: Result: -3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(multiplication_with_both_negative_parameters_gives_positive_result)
{
	freopen("mockoutput", "w", stdout);
	mul_fraction(-5, 5, -3, 2);
	mul_fraction(5, -5, 3, -2);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "-5/5 * (-3/2) should result in printing: Result: 3/2\\n");
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "5/-5 * (3/-2) should result in printing: Result: 3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(test_multiplication_with_hidden_data_when_on_server)
{
	run_hidden_tests(multiply, mul_fraction);
}
END_TEST

// Division
START_TEST(division_should_produce_correct_result_when_not_simplifying)
{
	freopen("mockoutput", "w", stdout);
	div_fraction(1, 1, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "1/1 / 2/3 should result in printing: Result: 3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(divison_should_simplify_when_possible)
{
	freopen("mockoutput", "w", stdout);
	div_fraction(5, 5, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "5/5 / 2/3 should result in printing: Result: 3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(division_should_not_work_when_there_is_a_zero_denominator)
{
	fail_unless(div_fraction(1, 0, 1, 5) == 0, "Passing a zero denominator polynomial to division should result in status code 0");
}
END_TEST

START_TEST(division_with_one_negative_parameter_gives_negative_result)
{
	freopen("mockoutput", "w", stdout);
	div_fraction(-5, 5, 2, 3);
	div_fraction(5, -5, 2, 3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -3/2\n"), "-5/5 / 2/3 should result in printing: Result: -3/2\\n");
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: -3/2\n"), "5/-5 / 2/3 should result in printing: Result: -3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(division_with_both_negative_parameters_gives_positive_result)
{
	freopen("mockoutput", "w", stdout);
	div_fraction(-5, 5, -2, 3);
	mul_fraction(-5, -5, -2, -3);
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Result: 3/2\n"), "-5/5 / (-2/3) should result in printing: Result: 3/2\\n");
	fail_unless(!strcmp(str, "Result: 3/2\n"), "-5/-5 / (-2/-3) should result in printing: Result: 3/2\\n");
	fclose(fp);	
}
END_TEST

START_TEST(test_division_with_hidden_data_when_on_server)
{
	run_hidden_tests(divide, div_fraction);
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

	// Addition
	tmc_register_test(s, adding_fractions_with_same_denominator_works, "8.0");
	tmc_register_test(s, adding_fractions_with_another_input, "8.0");

	tmc_register_test(s, adding_fractions_causes_simplification, "8.0");
	tmc_register_test(s, adding_fractions_with_zero_denominator_gives_error, "8.0");
	tmc_register_test(s, adding_one_fifth_to_one_tenth_should_produce_three_tenths, "8.0");
	tmc_register_test(s, adding_two_negative_signed_fractions_works_with_first_argument_negatives, "8.0");
	tmc_register_test(s, adding_two_negative_signed_fractions_with_both_argument_negatives, "8.0");
	tmc_register_test(s, adding_a_positive_value_to_a_larger_negative_value_keeps_result_negative, "8.0");

	// Subtraction
	tmc_register_test(s, subtracting_fractions_with_same_denominator_works, "9.0");
	tmc_register_test(s, subtracting_fractions_causes_simplification, "9.0");
	tmc_register_test(s, subtracting_fractions_with_zero_denominator_gives_error, "9.0");
	tmc_register_test(s, subtraction_with_second_negative_argument_works, "9.0");
	tmc_register_test(s, subtraction_with_both_values_negative, "9.0");

	// Multiplication
	tmc_register_test(s, multiplication_should_produce_correct_result_when_not_simplifying, "10.0");
	tmc_register_test(s, multiplication_should_simplify_when_possible, "10.0");
	tmc_register_test(s, multiplication_should_fail_when_there_is_a_zero_denominator, "10.0");
	tmc_register_test(s, multiplication_with_one_negative_parameter_gives_negative_result, "10.0");
	tmc_register_test(s, multiplication_with_both_negative_parameters_gives_positive_result, "10.0");

	// Division
	tmc_register_test(s, division_should_produce_correct_result_when_not_simplifying, "11.0");
	tmc_register_test(s, divison_should_simplify_when_possible, "11.0");
	tmc_register_test(s, division_should_not_work_when_there_is_a_zero_denominator, "11.0");
	tmc_register_test(s, division_with_one_negative_parameter_gives_negative_result, "11.0");
	tmc_register_test(s, division_with_both_negative_parameters_gives_positive_result, "11.0");

	// Add hidden tests
	tmc_register_test(s, test_adding_fractions_with_hidden_data_when_on_server, "8.0");
	tmc_register_test(s, test_subtraction_with_hidden_data_when_on_server, "9.0");
	tmc_register_test(s, test_multiplication_with_hidden_data_when_on_server, "10.0");
	tmc_register_test(s, test_division_with_hidden_data_when_on_server, "11.0");

	return tmc_run_tests(argc, argv, s);
}
