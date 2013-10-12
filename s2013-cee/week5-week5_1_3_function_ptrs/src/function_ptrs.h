#ifndef FUNCTIONPTRS_H
#define FUNCTIONPTRS_H

double maxi(double x, double  y, double (*fp)(double z));
int compare_gen(const void *block1, const void *block2,
        size_t elem_size, size_t block1size, size_t block2size,
        int (*compare_it)(const void *, const void *));
void print_gen(const void *block, size_t elem_size, size_t block_size, void (*print_it) (const void*));

#endif
