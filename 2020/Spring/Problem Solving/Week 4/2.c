#include <stdio.h>
#include <stdlib.h>

#define SWAP(x, y) ((x = x ^ y, y = x ^ y, x = x ^ y))

void quicksort(int arr[], int left, int right)
{
    int pivot, i, j, temp;
    if (left < right)
    {
        i = left, j = right + 1;
        pivot = arr[left];
        do
        {
            do
                i++;
            while (arr[i] > pivot);
            do
                j--;
            while (arr[j] < pivot);
            if (i < j)
            {
                if(arr[i] != arr[j])
                    SWAP(arr[i], arr[j]);
            }
        } while (i < j);
        if(arr[left] != arr[j])
            SWAP(arr[left], arr[j]);
        quicksort(arr, left, j - 1);
        quicksort(arr, j + 1, right);
    }

    return;
}

int numShoppingBag(int *weights, int size, int limit)
{
    int ans = 0, tempSum, tempIndex, equal_to_limit, i, j;

    quicksort(weights, 0, size - 1);                    //Greedy Algorithm을 위한 sort

    for (i = 0; i < size; i++)
    {
        if (weights[i] == -1)                           //이미 사용됨
            continue;

        if (weights[i] > limit)                         //limit보다 더 클 때
            return -1;

        if (weights[i] == limit)                        //limit의 무게와 일치
        {
            ans += 1;
            weights[i] = -1;
            continue;
        }

        tempSum = 0;                                    //임시합 저장용
        tempIndex = -1;                                 //임시합을 만족하는 index
        equal_to_limit = 0;                             //두 상자의 합으로 limit와 동일한 무게를 만들 수 있는지

        for (j = i + 1; j < size; j++)
        {
            if (weights[j] == -1)                       //이미 사용됨
                continue;

            int sum = weights[i] + weights[j];

            if (sum > limit)                            //합이 limit보다 큼
                continue;
            else if (sum == limit)                      //limit과 같으면 더 이상 비교 X
            {
                ans += 1;
                weights[i] = weights[j] = -1;
                equal_to_limit = 1;
                break;
            }
            else if (sum > tempSum)                     //limit보다는 작으면서 임시합보다는 큼
            {
                tempSum = sum;
                tempIndex = j;
            }
        }

        if (!equal_to_limit)                            //모든 합이 limit를 만족하지 못함
        {
            if (tempIndex == -1)                        //모든 sum이 limit 초과
                weights[i] = -1;
            else                                        //limit보다는 작지만 최적의 sum
                weights[i] = weights[tempIndex] = -1;
            ans += 1;
        }
    }

    return ans;
}

int main()
{
    int num;
    int limit;

    scanf("%d", &limit);
    scanf("%d", &num);

    int *weights = malloc(sizeof(int) * num);
    for (int i = 0; i < num; i++)
        scanf("%d", &weights[i]);

    printf("%d", numShoppingBag(weights, num, limit));

    return 0;
}