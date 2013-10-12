#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/writing_and_reading.h"

// SEE man dup

START_TEST(hello_world_printed_correctly)
{
	freopen("mockoutput", "w", stdout);
	hello_world();
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "Hello World! It's now year 2013\n"), "The printed did not match \"Hello World! It's now year 2013\\n\"");
	fclose(fp);
}
END_TEST

START_TEST(test_print_numbers)
{
	freopen("mockoutput", "w", stdout);
	print_numbers();
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [200];
	fgets(str, 200, fp);
	fail_unless(!strcmp(str, "12345.68    \n"), "Printing of number 12345.6789 was unsuccessful, check that the formatting is of correct length, on the right side and is followed by a line break");
	fgets(str, 200, fp);
  /*fail_unless(!strcmp(str, "   1.000000e+02f\n"), "100.00000002 was unsuccessfully printed, check that the correct number of decimal points were used, that number is on the right side of the formatted output and that it is followed by a line break");*/
	fail_unless(!strcmp(str, "   1.000000e+02\n"), "100.00000002 was unsuccessfully printed, check that the correct number of decimal points were used, that number is on the right side of the formatted output and that it is followed by a line break");
	fgets(str, 200, fp);
	fail_unless(!strcmp(str, "20 40\n"), "Wrong formatting was used, or space or line break was forgotten");
	fclose(fp);
}
END_TEST

START_TEST(test_read_file)
{
	freopen("mockoutput", "w", stdout);
	read_file();
	fclose(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "2 1.100000 0 c\n"), "One or more of the read variables was printed in an incorect format. Check that you are using the correct formatting and read the variables in the correct order. Remembrer, you should rear test/TEST1.txt");
	fclose(fp);
}
END_TEST

START_TEST(test_read_and_print_two_integers)
{
	FILE *mock_input = freopen("mockinput", "w+", stdin);
	fputs("2 1\n", mock_input);
	fseek(mock_input, 0, SEEK_SET);
	freopen("mockoutput", "w", stdout);
	read_and_print_two_integers();
	fflush(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "2 1\n"), "When giving the program the input 2 1, \"2 1\\n\" shold be printed");
	fclose(fp);
}
END_TEST

START_TEST(test_read_and_print_five_chars)
{
	FILE *mock_input = freopen("mockinput", "w+", stdin);
	fputs("a b c d e", mock_input);
	fseek(mock_input, 0, SEEK_SET);
	freopen("mockoutput", "w", stdout);
	read_and_print_five_chars();
	fflush(stdout);
	FILE *fp = fopen("mockoutput", "r");
	char str [100];
	fgets(str, 100, fp);
	fail_unless(!strcmp(str, "a b c d e\n"), "When giving input a b c d e, \"a b c d e\\n\" should be printed");
	fclose(fp);
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

	tmc_register_test(s, hello_world_printed_correctly, "1.0");
	tmc_register_test(s, test_print_numbers, "2.0");
	tmc_register_test(s, test_read_and_print_two_integers, "3.0");
	tmc_register_test(s, test_read_and_print_five_chars, "3.0");
	tmc_register_test(s, test_read_file, "4.0");

	return tmc_run_tests(argc, argv, s);
}
