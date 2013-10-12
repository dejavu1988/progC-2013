#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polynomial.h"

/* GCD is useful in this exercise, copy the iterative GCD from the SimpleMath exercise set */
int gcd(int a, int b) {
  if(b == 0) return a;
  else return gcd(b, a%b);
}



/* Exercise 8 */
int add_fraction(int a, int c, int b, int d) {
    int denom, numer, g, s;
    if(!c || !d) return 0;
    numer = a*d + b*c;    
    denom = c*d;
    s = (numer*denom >= 0)?1:-1;
    numer = abs(numer);
    denom = abs(denom);
    g = gcd(numer, denom);
    //printf("%d\t", g);
    denom = denom / g;
    numer = numer / g * s;
    if(numer)   printf("Result: %d/%d\n", numer, denom);
    else printf("Result: %d\n", numer);
	return 1;
}

/* Exercise 9 */
int sub_fraction(int a, int c, int b, int d) {
    return add_fraction(a, c, -b, d);
}

/* Exercise 10 */
int mul_fraction(int a, int c, int b, int d) {
    int denom, numer, g, s;
    if(!c || !d) return 0;
    numer = a*b;    
    denom = c*d;
    s = (numer*denom >= 0)?1:-1;
    numer = abs(numer);
    denom = abs(denom);
    g = gcd(numer, denom);
    //printf("%d\t", g);
    denom = denom / g;
    numer = numer / g * s;
    if(numer)   printf("Result: %d/%d\n", numer, denom);
    else printf("Result: %d\n", numer);
	return 1;
}

/* Exercise 11 */
int div_fraction(int a, int c, int b, int d) {
    
	return mul_fraction(a, c, d, b);
}