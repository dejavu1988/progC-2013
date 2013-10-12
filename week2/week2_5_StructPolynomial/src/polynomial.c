#include <stdio.h>
#include <stdlib.h>
#include "polynomial.h"

int gcd(int a, int b) {
  if(b == 0) return a;
  else return gcd(b, a%b);
}


/* Exercise 5 */
fraction add_fraction(fraction r, fraction s) {
    int numer, denom, g, k;
    if(!r.denominator * s.denominator) return (fraction) {0,0};
    numer = r.numerator * s.denominator + r.denominator * s.numerator;
    denom = r.denominator * s.denominator;
    k = (numer * denom >= 0)?1:-1;
    numer = abs(numer);
    denom = abs(denom);
    g = gcd(numer, denom);
    numer = numer / g * k;
    denom = denom / g;
    return (fraction) {numer, denom};
}
