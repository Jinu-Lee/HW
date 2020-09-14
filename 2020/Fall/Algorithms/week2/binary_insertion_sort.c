#include <stdio.h>

int binary_search(int* array, int key, int left, int right)
{
    int mid;
    while(left <= right) {
        mid = (left + right) / 2;
        if (array[mid] < key) {
            left = mid + 1;
        }
        else if (array[mid] > key) {
            right = mid - 1;
        }
        else {
            return mid;
        }
    }
    return left;
}

void binary_insertion_sort(int* array, int size)
{
    int i, j, temp, searched;
    for(i = 1; i < size; i++) {
        temp = array[i];
        j = i - 1;
        searched = binary_search(array, temp, 0, j);
        for(; j >= searched; j--) {
            array[j + 1] = array[j];
        }
        array[j + 1] = temp;
    }

    return;
}

int main()
{
    int size;
    scanf("%d", &size);

    int array[10000];
    for(int i = 0; i < size; i++) {
        scanf("%d", &array[i]);
    }

    binary_insertion_sort(array, size);

    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
}