#include <stdlib.h>
#include <stdio.h>
#include "function_ptrs.h"

double maxi(double x, double y, double (*fp)(double z)) {
    double xi, yi;
    xi = fp(x);
    yi = fp(y);
    return (xi >= yi)?xi:yi;
}

int compare_gen(const void *block1, const void *block2,
        size_t elem_size, size_t block1size, size_t block2size,
        int (*compare_it)(const void*, const void*)) {
    const void *p1, *p2;
    int res;
    if((p1 = block1) && (p2 = block2)){
        for(;p1 < block1+block1size && p2 < block2+block2size;p1+=elem_size, p2+=elem_size){
            if((res = compare_it(p1,p2)) != 0)
                return res;
        }
        if(block1size == block2size)
            return 0;
        else if(block1size > block2size)
            return -1;
        else 
            return 1;
    }
    return 0;
}

void print_gen(const void *block, size_t elem_size, size_t block_size, void (*print_it) (const void*)) {
    const void *p;
    if((p = block)){
        for(; p < block+block_size; p+=elem_size){
            print_it(p);
        }
    }
}
