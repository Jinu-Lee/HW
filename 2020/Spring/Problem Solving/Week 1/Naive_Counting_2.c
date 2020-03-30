#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int main()
{
    FILE *f = fopen("input.txt", "r");
    char temp;
    int N = 0, max = 0, min = INT_MAX, index, i;

    /* 1. 파일에서 읽기 & 최대, 최소 저장 */
    while ((temp = fgetc(f)) != EOF)
        if (temp == ' ')
            N++;
    rewind(f);

    int *cards = malloc(sizeof(int) * (++N));
    for (i = 0; i < N; i++)
    {
        fscanf(f, "%d", &cards[i]);
        max = (cards[i] > max) ? cards[i] : max;
        min = (cards[i] < min) ? cards[i] : min;
    }

    /* 2. 첫 번째 간추리기 */
    int n = ceil(sqrt(max - min));
    int *count = calloc(n, sizeof(int));
    for (i = 0; i < N; i++)
        count[(cards[i] - min) % n]++;

    for (i = 0; i < n; i++)
    {
        if (count[i] % 2 == 1)
            index = i;
        count[i] = 0;
    }

    /* 3. 두 번째 간추리기 */
    for (i = 0; i < N; i++)
        if ((cards[i] - min) % n == index)
            count[(cards[i] - index - min) / n]++;

    for (i = 0; i < n; i++)
        if (count[i] == 1)
            break;

    printf("The person with the number %d is the solo player\n", i * n + min + index);

    free(cards);
    free(count);
    fclose(f);
    return 0;
}