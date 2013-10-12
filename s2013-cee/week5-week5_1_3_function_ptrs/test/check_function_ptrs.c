#include <check.h>
#include "tmc-check.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "../src/function_ptrs.h"

// maxi tests
double square(double x) { return x * x; }

START_TEST(test_maxi_with_square_function)
{
    double result = maxi(2, 3, (&square));
    fail_unless(result == 9, "When squaring 2 and 3, 9 should be the max of the results");

    result = maxi(5, 3, (&square));
    fail_unless(result == 25, "When squaring 5 and 3, 25 should be the max of the results");
}
END_TEST

double negative(double x) { return -1 * x; }

START_TEST(test_maxi_with_negative)
{
    double result = maxi(1, -1, (&negative));
    fail_unless(result == 1, "When giving 1 and -1 to a function doing x = -x, 1 should be the max");

    result = maxi(-9, -2, (&negative));
    fail_unless(result == 9, "When giving -9 and -2 to a function doing x = -x, 9 should be the max");
}
END_TEST

int compare_ints(const void* v1, const void* v2) {
    int *a = (int*)v1;
    int *b = (int*)v2;

    if (*a == *b) return 0;
    else if (*a > *b) return 1;
    else return -1;
}

// compare_gen tests
START_TEST(test_compare_gen_with_int_arrays)
{
    int array1 [10] = {10};
    int array2 [10] = {10};
    int array3 [10] = {10};
    int array4 [9] = {10};
    for (int i = 0; i < 10; i++) {
        array1[i] = 10;
        array2[i] = 10;
        array3[i] = 10;
        if (i < 9) array4[i] = 10;
    }
    array3[5] = 9;
    int result;
    result = compare_gen(array1, array2, sizeof(int), sizeof(array1), sizeof(array2), (&compare_ints)); 
    fail_unless(result == 0, "When comparing two identical integer arrays, result should equal 0 (this is lexicographical order");

    result = compare_gen(array1, array3, sizeof(int), sizeof(array1), sizeof(array3), (&compare_ints));
    fail_unless(result == 1, "When comparing and array with another array with one smaller value than in the other, the first parameter should be larger, i.e., the function should return 1");

    result = compare_gen(array1, array4, sizeof(int), sizeof(array1), sizeof(array4), (&compare_ints));
    fail_unless(result == -1, "When comparing a longer array with a shorter array the lexicographical order should be -1");
}
END_TEST


int compare_chars(const void *v1, const void *v2) {
    char a = *(char*)v1;
    char b = *(char*)v2;

    if (a > b) return 1;
    else if (a == b) return 0;
    else return -1;
}

START_TEST(test_compare_gen_with_chars)
{
    char talon [5] = "talon";
    char talo [4] = "talo";
    char taco [4] = "taco";
    char talos [5] = "talos";

    int result = compare_gen(talon, talo, sizeof(char), sizeof(talon), sizeof(talo), (&compare_chars));
    fail_unless(result == -1, "Comparing a longer otherwise identical string with longer one should yield -1");

    result = compare_gen(talo, taco, sizeof(char), sizeof(talo), sizeof(taco), (&compare_chars));
    fail_unless(result == 1, "Comparing talo with taco should yield 1");

    result = compare_gen(talon, talos, sizeof(char), sizeof(talon), sizeof(talos), (&compare_chars));
    fail_unless(result == -1, "Comparing talon with talos should yield -1");
}
END_TEST

void print_ints(const void *v) {
    int i = *(int*)v;
    printf("%i\n", i);
}

START_TEST(test_print_gen_with_integers)
{
    int array [5];
    for (int i = 1; i <= 5; i++) array[i-1] = i;
    freopen("mockoutput.txt", "w", stdout);
    print_gen(array, sizeof(int), sizeof(array), (&print_ints));
    fclose(stdout);
    
    FILE *fp = fopen("mockoutput.txt", "r");
    for (int i = 0; i < 5; i++) {
        char str [10];
        char cmp_str [10];
        fgets(str, 10, fp);
        sprintf(cmp_str, "%i\n", i+1);
        char tooltip [30];
        char *d;
        char *s;
        sprintf(tooltip, "Expected: %s, Got: %s", cmp_str, str);
        for (s=d=tooltip;(*d=*s);d+=(*s++!='\n'));
        fail_unless(strcmp(str, cmp_str) == 0, tooltip);
    }
}
END_TEST

void print_chars(const void *v) {
    char c = *(char*)v;
    printf("%c\n", c);
}

START_TEST(test_print_gen_with_characters)
{
    char array [5] = {'a', 'b', 'c', 'd', 'e'};
    freopen("mockoutput.txt", "w", stdout);
    print_gen(array, sizeof(char), sizeof(array), (&print_chars));
    fclose(stdout);
    
    FILE *fp = fopen("mockoutput.txt", "r");
    for (int i = 0; i < 5; i++) {
        char str [10];
        char cmp_str [10];
        fgets(str, 10, fp);
        sprintf(cmp_str, "%c\n", array[i]);
        char tooltip [30];
        char *d;
        char *s;
        sprintf(tooltip, "Expected: %s, Got: %s", cmp_str, str);
        for (s=d=tooltip;(*d=*s);d+=(*s++!='\n'));
        fail_unless(strcmp(str, cmp_str) == 0, tooltip);
    }
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

    tmc_register_test(s, test_maxi_with_square_function, "w5-1.-1");
    tmc_register_test(s, test_maxi_with_negative, "w5-1.0");
    tmc_register_test(s, test_compare_gen_with_int_arrays, "w5-2.0");
    tmc_register_test(s, test_compare_gen_with_chars, "w5-2.0");
    tmc_register_test(s, test_print_gen_with_integers, "w5-3.0");
    tmc_register_test(s, test_print_gen_with_characters, "w5-3.0");

	return tmc_run_tests(argc, argv, s);
}
