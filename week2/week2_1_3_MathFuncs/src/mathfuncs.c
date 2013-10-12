#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathfuncs.h"

/* Exercise 1 */
void save_sum(int* x, int* y, int* z) {
    
    *z = *x + *y;
}

/* Exercise 2 */
double average(double* a, int len) {
    double acc = 0;
    for(int i = 0; i < len; i++){
        acc += *(a+i);
    }
  return acc/len;
}

/* Exercise 3 */
int substitute_and_sum(int* a, int len) {    
    short acc = 0;
    short *p;
    p = (short*)a;
    *(a + (int) floor(len/2)) = 2;
    for(int i = 0; i < 2*len; i++){
        acc += *(p+i);
    }
  return acc;
}