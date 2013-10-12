#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathfuncs.h"

/* Exercise 5 a */
double sum_of_absolutes(double x, double y)
{
	return fabs(x) + fabs(y);
}

/* Exercise 5 b */
int sum_of_rounded(double x, double y) 
{
	return ceil(x) + ceil(y);
}

/* Exercise 5 c */
int sum_of_characters(char a, char b)
{
	return a + b;
}

/* Exercise 6 */
int gcd(int a, int b)
{
    if(b == 0) return a;
    else return gcd(b, a%b);
}

/* Exercise 7 */
int gcd_iterative(int a, int b) {
    int r, x, y;
    x = a;
    y = b;
    while(y!=0){
        r = x%y;
        x = y;
        y = r;
    }    
	return x;
}
