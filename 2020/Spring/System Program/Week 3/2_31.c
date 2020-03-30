#include <stdio.h>
#include <limits.h>

/* Determine whether arguments can be added without overflow */
/* WARNING: This code is buggy. */
int tadd_ok(int x, int y)
{
    int sum = x+y;
	return (sum-x == y) && (sum-y == x);
}

int main()
{
    printf("%d\n", tadd_ok(INT_MAX, 1));
}