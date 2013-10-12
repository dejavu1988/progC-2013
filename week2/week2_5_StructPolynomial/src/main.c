#include"polynomial.h"
#include<stdio.h>

int main()
{
    fraction r;
    r = add_fraction((fraction){-5,6},(fraction){1,2});
    printf("%d/%d\n", r.numerator, r.denominator);
	return 1;
}