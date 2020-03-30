#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/* Determine whether arguments can be added without overflow */
int tmul_ok(int x, int y) {
    int64_t result = (int64_t)x * y;
    return result == (int) result;
}

int main()
{
    printf("%d", tmul_ok(INT32_MAX, 2));
}