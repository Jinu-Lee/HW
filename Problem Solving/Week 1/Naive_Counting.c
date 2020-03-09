#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t start, end;
    start = clock();

    /* ------------------- 실행 코드 ------------------- */
    FILE *f = fopen("input.txt", "r");
    char temp;
    int N = 0, max = 0, num, i;

    while ((temp = fgetc(f)) != EOF)
        if (temp == ' ')
            N++;
    rewind(f);

    int *cards = malloc(sizeof(int) * (++N));
    for (i = 0; i < N; i++)
    {
        fscanf(f, "%d", &cards[i]);
        max = (cards[i] > max) ? cards[i] : max;
    }

    int *count = calloc((max + 1), sizeof(int));
    for (i = 0; i < N; i++)
        count[cards[i]]++;

    for (i = 0; i <= max; i++)
        if (count[i] == 1)
        {
            num = i;
            break;
        }

    printf("The person with the number %d is the solo player", num);
    /* ------------------------------------------------ */

    end = clock();
    printf("\n%lfms", (double)(end - start));

    free(cards);
    free(count);
    fclose(f);
    return 0;
}