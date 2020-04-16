#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0

int dp(int* gasStation, int* result_arr, int k, int n) 
{
    int result = 0, i;
    if(k == n)
        return (result_arr[k] = 1);
    if(result_arr[k] != -1)
        return result_arr[k];
    for(i = 1; i <= gasStation[k]; i++) {
        result |= dp(gasStation, result_arr, k + i, n);
        if(result == 1)
            break;
    }
    return (result_arr[k] = result);
}

int isReachable(int *gasStation, int N)
{
    int res[2500] = {0,}, i;
    for(i = 0; i < N; i++)
        res[i] = -1;
    return dp(gasStation, res, 0, N - 1);
}

int main()
{
    char *input = NULL;
    int in_tmp, N = 0;
    size_t size;
    char *ptr;
    int gasStation[2500];
    int result = true;

    getline(&input, &size, stdin);

    ptr = strtok(input, " ");
    while (ptr != NULL)
    {
        in_tmp = atoi(ptr);
        gasStation[N++] = in_tmp;

        ptr = strtok(NULL, " ");
    }

    result = isReachable(gasStation, N);
    printf("%s\n", (result ? "true" : "false"));

    return 0;
}