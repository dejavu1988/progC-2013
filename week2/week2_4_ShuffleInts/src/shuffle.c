#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shuffle.h"

/* Exercise 4 */
void shuffle_ints(int* array, int len) {
    int i;
    for (i = len; i >= 1; i--) {
        int idx = rand() % i;
        int tmp = array[i - 1];
        array[i - 1] = array[idx];
        array[idx] = tmp;
    }
}
