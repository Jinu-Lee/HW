#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* obj;
    int start, len;
} Key;

int compare_same(char* w1, char* w2, int len) {
    int i;
    for (i = 0; (i < len) && (w1[i] == w2[i]); i++) {}

    char c1 = w1[i], c2 = w2[i];
    if (c1 > c2) 
        return -1;
    else if (c1 < c2)
        return 1;
    return 0;
}

int compare(Key* k1, Key* k2) {
    char temp1[16], temp2[16];
    char* w1 = k1->obj + k1->start, *w2 = k2->obj + k2->start;
    int i, len1 = k1->len, len2 = k2->len;

    strncpy(temp1, w1, len1);
    strncpy(temp2, w2, len2);
    temp1[len1] = '\0';
    temp2[len2] = '\0';

    for (i = 0; i < len1; i++) {
        if (temp1[i] <= 'Z') temp1[i] += 32;
    }
    for (i = 0; i < len2; i++) {
        if (temp2[i] <= 'Z') temp2[i] += 32;
    }

    for (i = 0; (temp1[i] == temp2[i]) && temp2[i] != '\0'; i++) {}

    char c1 = temp1[i], c2 = temp2[i];
    if (c1 < c2) 
        return -1;
    else if (c1 > c2)
        return 1;
    return compare_same(w1, w2, len1);
}

void merge(Key* arr, int left, int mid, int right) {
    int n_left = mid - left + 1, n_right = right - mid;
    Key* left_arr = malloc(sizeof(Key) * n_left);
    Key* right_arr = malloc(sizeof(Key) * n_right);
    Key* left_ptr, *right_ptr, *arr_ptr, *left_end = left_arr + n_left, *right_end = right_arr + n_right;

    for (left_ptr = left_arr, arr_ptr = arr + left; left_ptr != left_end; left_ptr++, arr_ptr++) {
        left_ptr->obj = arr_ptr->obj;
        left_ptr->start = arr_ptr->start;
        left_ptr->len = arr_ptr->len;
    }

    for (right_ptr = right_arr, arr_ptr = arr + mid + 1; right_ptr != right_end; right_ptr++, arr_ptr++) {
        right_ptr->obj = arr_ptr->obj;
        right_ptr->start = arr_ptr->start;
        right_ptr->len = arr_ptr->len;
    }

    for (left_ptr = left_arr, right_ptr = right_arr, arr_ptr = arr + left; (left_ptr != left_end) && (right_ptr != right_end); arr_ptr++) {
        if (compare(left_ptr, right_ptr) <= 0) {
            arr_ptr->obj = left_ptr->obj;
            arr_ptr->start = left_ptr->start;
            arr_ptr->len = (left_ptr++)->len;
        }
        else {
            arr_ptr->obj = right_ptr->obj;
            arr_ptr->start = right_ptr->start;
            arr_ptr->len = (right_ptr++)->len;
        }
    }

    for (; left_ptr != left_end; left_ptr++, arr_ptr++) {
        arr_ptr->obj = left_ptr->obj;
        arr_ptr->start = left_ptr->start;
        arr_ptr->len = left_ptr->len;
    }

    for (; right_ptr != right_end; right_ptr++, arr_ptr++) {
        arr_ptr->obj = right_ptr->obj;
        arr_ptr->start = right_ptr->start;
        arr_ptr->len = right_ptr->len;
    }

    free(left_arr);
    free(right_arr);
}

void merge_sort(Key* arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main(int argc, char* argv[])
{
    //FILE* input = fopen("hw1_input.txt", "r");
    //FILE* output = fopen("hw1_output.txt", "w");

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    Key* keys; 
    char buf[165];
    int n, key, t = 1;

    //read n
    fscanf(input, "%d ", &n);
    fgetc(input); fgetc(input);

    //determine which attribute is the key
    fgets(buf, 165, input);
    for (int i = 0; i <= 160; i++) {
        char temp = buf[i];
        if ((temp == '(') && (buf[i + 1] == 'K') && (buf[i + 2] == 'e') && (buf[i + 3] == 'y') && (buf[i + 4] == ')')) {
            key = t;
        }
        if (temp == ':') {
            t++;
        }
    }

    keys = (Key*) malloc(sizeof(Key) * n);

    //read object one by one
    fgetc(input); fgetc(input);
    for (int i = 0, idx = 0; i < n; i++) {
        int size = 16 * t + 1, prev_key, curr_key = -1, cnt = 0;
        char* buf = (char*) malloc(size);
        keys[i].obj = buf;
        fgets(buf, size, input);
        
        for (char* p = buf; *p != '\0'; p++) {
            if (*p == '\n') *p = '\0';
        }

        for (int j = 0; j < size; j++) {
            if (buf[j] == ':' || buf[j] == '\0') {
                prev_key = curr_key;
                curr_key = j;
                cnt++;
            }
            if (key == cnt) {
                int len = curr_key - prev_key - 1;
                keys[i].start = prev_key + 1;
                keys[i].len = len;
                break;
            }
        }
    }

    merge_sort(keys, 0, n - 1);

    for (int i = 0; i < n; i++) {
        fputs(keys[i].obj, output);
        if (i != n - 1) {
            fputc('\n', output);
        }
        free(keys[i].obj);
    }
    free(keys);

    fclose(input);
    fclose(output);

    return 0;
}