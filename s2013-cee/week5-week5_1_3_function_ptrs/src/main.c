#include <stdlib.h>
#include <stdio.h>
#include "function_ptrs.h"

int compare_ints(const void* v1, const void* v2) {
    int *a = (int*)v1;
    int *b = (int*)v2;

    if (*a == *b) return 0;
    else if (*a > *b) return 1;
    else return -1;
}

int main()
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
    printf("%d %d %d", sizeof(array1), sizeof(array2), result);
    return 0;
}
