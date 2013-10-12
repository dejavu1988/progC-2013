#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "writing_and_reading.h"

/* Exercise 1 */
void hello_world() {
	printf("Hello World! It's now year %4d\n", 2013);
}

/* Exercise 2 */
void print_numbers() {
	printf("%-12.2f\n", 12345.6789);
	printf("%15e\n", 100.00000002);
	printf("%x %o\n", 32, 32);
}

/* Exercise 3 a */
void read_and_print_two_integers() {
    int i, j;
    scanf("%d %d", &i, &j);
    printf("%d %d\n", i, j);
}

/* Exercise 3 b */
void read_and_print_five_chars() {
    char a, b, c, d, e;
    scanf("%c %c %c %c %c", &a, &b, &c, &d, &e);
    printf("%c %c %c %c %c\n", a, b, c, d, e);
}

/* Exercise 4 */
void read_file() {
    FILE* f;
    int a, c;
    char d;
    double b;
    double x, y, z;
    if ((f = fopen("test/TEST1.txt", "r")) == NULL) {
        fprintf(stderr, "can't read %s\n", "TEST1.txt");
    }
    
    if (fscanf(f, "%d %lf %x %c", &a, &b, &c, &d) != 4) {
        fprintf(stderr, "File read failed\n");
        return EXIT_FAILURE;
    }
    printf("%d %lf %x %c\n", a, b, c, d);
}
