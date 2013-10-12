#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "tmc-check.h"
#include "../src/fun_with_files.h"

// Helpers
FILE *write_to_stdin(char *str) {
	FILE *input = freopen("mockinput", "w+", stdin);
	fputs(str, input);
	fseek(input, 0, SEEK_SET);
	return input;
}

// Appending tests
START_TEST(test_appending_hello_world_to_an_empty_file_works)
{
	FILE *input = write_to_stdin("Hello world!");

	int status = append_string("test/empty_file.txt");

	FILE *fp = fopen("test/empty_file.txt", "r");
	char line [20];
	fgets(line, 100, fp);
	fclose(fp);

	remove("test/empty_file.txt");

	int cmp = strcmp(line, "Hello world!");
	fail_unless(status == 0, "Status should be zero");
	fail_unless(cmp == 0, "The file should contain Hello world! when appending to empty file");
	fclose(input);
}
END_TEST

START_TEST(test_appending_to_file_that_already_says_hello)
{
	FILE *fp = fopen("test/hello_file.txt", "w+");
	fputs("Hello", fp);
	fclose(fp);

	FILE *input = write_to_stdin(" world!");

	int status = append_string("test/hello_file.txt");

	fp = fopen("test/hello_file.txt", "r");
	char line [20];
	fgets(line, 100, fp);
	fclose(fp);

	remove("test/hello_file.txt");

	int cmp = strcmp(line, "Hello world!");
	fail_unless(status == 0, "Status should be zero");
	fail_unless(cmp == 0, "The file shoul contain Hello world! when appending \" world!\" to a file containing \"Hello\"");
	fclose(input);
}
END_TEST

// Remove empty lines tests
START_TEST(test_remove_empty_lines_leaves_file_with_no_empty_lines_intact)
{
	FILE *fp = fopen("test/no_empty_lines.txt", "w+");
	fputs("Not empty\n", fp);
	fputs("Not empty\n", fp);
	fputs("Not empty\n", fp);
	fputs("Not empty either\n", fp);
	fclose(fp);

	remove_empty_lines("test/no_empty_lines.txt");

	int changed = 0;
	fp = fopen("test/no_empty_lines.txt", "r");
	char buffer [20];
	fgets(buffer, 20, fp);
	if (strcmp(buffer, "Not empty\n") != 0) changed = 1;
	fgets(buffer, 20, fp);
	if (strcmp(buffer, "Not empty\n") != 0) changed = 1;
	fgets(buffer, 20, fp);
	if (strcmp(buffer, "Not empty\n") != 0) changed = 1;
	fgets(buffer, 20, fp);
	if (strcmp(buffer, "Not empty either\n") != 0) changed = 1;

	remove("test/no_empty_lines.txt");
	fail_unless(!changed, "A file with no empty lines should stay intact");
}
END_TEST

START_TEST(test_remove_empty_lines_works_when_removing_first_line)
{
	int content_rows = 10;
	FILE *fp = fopen("test/delete_first_line.txt", "w+");
	fputs("\n", fp);
	for (int i = 0; i < content_rows; i++) {
		fprintf(fp, "Content of line %i\n", i);
	}
	fclose(fp);

	remove_empty_lines("test/delete_first_line.txt");

	int changed = 0;
	fp = fopen("test/delete_first_line.txt", "r");
	for (int i = 0; i < content_rows; i++) {
		int row_id;
		fscanf(fp, "Content of line %i\n", &row_id);
		if (row_id != i) changed = 1;
	}
	remove("test/delete_first_line.txt");
	fail_if(changed, "Line should contain all the same rows except not the first line");
}
END_TEST

START_TEST(test_remove_empty_lines_works_when_removing_even_lines)
{
	int content_rows = 10;
	FILE *fp = fopen("test/delete_even_lines.txt", "w+");
	for (int i = 0; i < content_rows; i++) {
		if (i % 2 == 0) {
			fputs("\n", fp);
		} else {
			fprintf(fp, "Content of line %i\n", i);	
		}
	}
	fclose(fp);

	remove_empty_lines("test/delete_even_lines.txt");

	int changed = 0;
	fp = fopen("test/delete_even_lines.txt", "r");
	for (int i = 1; i < content_rows; i+=2) {
		int row_id;
		fscanf(fp, "Content of line %i\n", &row_id);
		if (row_id != i) changed = 1;
	}
	remove("test/delete_even_lines.txt");
	fail_if(changed, "Line should contain all the same rows except not the first line");
}
END_TEST

START_TEST(test_remove_empty_lines_works_when_removing_odd_lines)
{
int content_rows = 10;
	FILE *fp = fopen("test/delete_odd_lines.txt", "w+");
	for (int i = 0; i <= content_rows; i++) {
		if (i % 2 == 1) {
			fputs("\n", fp);
		} else {
			fprintf(fp, "Content of line %i\n", i);	
		}
	}
	fclose(fp);

	remove_empty_lines("test/delete_odd_lines.txt");

	int changed = 0;
	fp = fopen("test/delete_odd_lines.txt", "r");
	for (int i = 0; i <= content_rows; i+=2) {
		int row_id;
		fscanf(fp, "Content of line %i\n", &row_id);
		if (row_id != i) {
			changed = 1;
		}
	}
	remove("test/delete_odd_lines.txt");
	fail_if(changed, "Line should contain all the same rows except not the first line");
}
END_TEST

START_TEST(test_remove_empty_lines_works_when_removing_odd_lines_with_windows_lb)
{
int content_rows = 10;
	FILE *fp = fopen("test/delete_odd_lines.txt", "w+");
	for (int i = 0; i <= content_rows; i++) {
		if (i % 2 == 1) {
			fputs("\t", fp);
		} else {
			fprintf(fp, "Content of line %i\n", i);	
		}
	}
	fclose(fp);

	remove_empty_lines("test/delete_odd_lines.txt");

	int changed = 0;
	fp = fopen("test/delete_odd_lines.txt", "r");
	for (int i = 0; i <= content_rows; i+=2) {
		int row_id;
		fscanf(fp, "Content of line %i\n", &row_id);
		if (row_id != i) {
			changed = 1;
		}
	}
	remove("test/delete_odd_lines.txt");
	fail_if(changed, "Line should contain all the same rows except not the first line");
}
END_TEST

// Tests for the counting function
START_TEST (test_character_counting_with_empty_file)
{
	int count = count_characters_in_file("test/empty.data");
	fail_unless(count == 0, "File named empty.data has no characters.");
}
END_TEST

START_TEST (test_character_counting_with_file_containing_three_characters_including_newline)
{
	int count = count_characters_in_file("test/three_characters.data");
	fail_unless(count == 3, "File named three_characters has exactly 3 characters, including a newline.");
}
END_TEST

START_TEST (test_chraracter_counting_with_file_containing_6_characters_including_spaces_on_three_lines)
{
	int count = count_characters_in_file("test/divided_example.data");
	fail_unless(count == 6, "File contains exactly 6 characters. Check that your function take into account line spaces and newlines.");
}
END_TEST

int main(int argc, const char* argv[]) {
	Suite *s = suite_create("FunWithFiles tests");

	// Appending tests
	tmc_register_test(s, test_appending_hello_world_to_an_empty_file_works, "w3-1.0");
	tmc_register_test(s, test_appending_to_file_that_already_says_hello, "w3-1.0");

	// Remove empty lines tests
	tmc_register_test(s, test_remove_empty_lines_leaves_file_with_no_empty_lines_intact, "w3-2.0");
	tmc_register_test(s, test_remove_empty_lines_works_when_removing_first_line, "w3-2.0");
	tmc_register_test(s, test_remove_empty_lines_works_when_removing_even_lines, "w3-2.0");
	tmc_register_test(s, test_remove_empty_lines_works_when_removing_odd_lines, "w3-2.0");
	tmc_register_test(s, test_remove_empty_lines_works_when_removing_odd_lines_with_windows_lb, "w3-2.0");

	// Count characters
	tmc_register_test(s, test_character_counting_with_empty_file, "w3-3.0");
	tmc_register_test(s, test_character_counting_with_file_containing_three_characters_including_newline, "w3-3.0");
	tmc_register_test(s, test_chraracter_counting_with_file_containing_6_characters_including_spaces_on_three_lines, "w3-3.0");

	return tmc_run_tests(argc, argv, s);
}
