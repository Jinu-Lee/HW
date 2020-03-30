#include <stdio.h>
#include <limits.h>

/* Determine whether arguments can be added without overflow */
int tadd_ok(int x, int y) {
    int sum = x + y;
    int positive_overflow = x >= 0 && y >= 0 && sum < 0; 
    int negative_overflow = x < 0 && y < 0 && sum >= 0;
    return !positive_overflow && !negative_overflow;
}

int main()
{
    printf("%d\n",tadd_ok(INT_MAX, 1));
}