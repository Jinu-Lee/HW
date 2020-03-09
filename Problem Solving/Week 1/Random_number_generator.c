#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    FILE *f = fopen("input.txt", "w");

    srand(time(NULL));

    int i;
    for(i=0; i <1000; i++)
        fprintf(f, "%d ", rand());

    fclose(f);

    return 0;

}