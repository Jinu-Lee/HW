#include <stdio.h>
#include <limits.h>


int main()
{
    int x = INT_MIN;
    int y = INT_MAX;
    unsigned ux = (unsigned) x;
    unsigned uy = (unsigned) y;

    printf("0x%08x 0x%08x", x, 2 * x);
}