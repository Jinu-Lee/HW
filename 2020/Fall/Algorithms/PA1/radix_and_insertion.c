#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* obj;
    int start, len;
} Key;

int char_to_idx(char c) {
    if (c == ' ') return 0;
    else if (c <= '9') return ((int) c - 47);
    else if (c <= 'Z') return ((int) c - 54);
    return ((int) c - 86);
}

char check_valid_pos(Key* key, int idx, int d) {
    if (d < key[idx].len) return char_to_idx(key[idx].obj[key[idx].start + d]);
    return -1;
}

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

void insertion_sort(Key* keys, int n) {
    Key temp;
    int j;
    for (int i = 1; i < n; i++) {
        temp.obj = keys[i].obj;
        temp.start = keys[i].start;
        temp.len = keys[i].len;

        for (j = i - 1; j >= 0; j--) {
            if (compare(&keys[j], &temp) > 0) {
                keys[j + 1].obj = keys[j].obj;
                keys[j + 1].start = keys[j].start;
                keys[j + 1].len = keys[j].len;
            }
            else {
                break;
            }
        }

        keys[j + 1].obj = temp.obj;
        keys[j + 1].start = temp.start;
        keys[j + 1].len = temp.len;
    }
}

void radix_sort(Key* keys, Key* temp, int low, int high, int d) {
    if (high <= low) return;
    int bucket[39] = {0, }, cnt = high - low + 1;
    char idx;
    Key* dest, *src = &keys[low];

    for (int i = low; i <= high; i++) {
        idx = check_valid_pos(keys, i, d);
        bucket[idx + 2]++;
    }

    for (int i = 0; i < 38; i++) {
        bucket[i + 1] += bucket[i];
    }

    for (int i = low; i <= high; i++, src++) {
        idx = check_valid_pos(keys, i, d);
        dest = &temp[bucket[idx + 1]];
        dest->obj = src->obj;
        dest->len = src->len;
        dest->start = src->start;
        bucket[idx + 1]++;
    }

    for (dest = &keys[low], src = &temp[0]; cnt--; dest++, src++) {
        dest->obj = src->obj;
        dest->len = src->len;
        dest->start = src->start;
    }

    for (int i = 0; i < 37; i++) {
        radix_sort(keys, temp, low + bucket[i], low + bucket[i + 1] - 1, d + 1);
    }
}

int main(int argc, char* argv[])
{
    //FILE* input = fopen("hw1_input.txt", "r");
    //FILE* output = fopen("hw1_output.txt", "w");

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    Key* keys, *temp; 
    char buf[165];
    int n, key, t = 1, min = 16;

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
    for (int i = 0; i < n; i++) {
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
                if (len < min) min = len;
                keys[i].start = prev_key + 1;
                keys[i].len = len;
                break;
            }
        }
    }

    temp = (Key*) malloc(sizeof(Key) * n);
    radix_sort(keys, temp, 0, n - 1, 0);
    insertion_sort(keys, n);

    for (int i = 0; i < n; i++) {
        fputs(keys[i].obj, output);
        if (i != n - 1) {
            fputc('\n', output);
        }
        free(keys[i].obj);
    }

    free(keys);
    free(temp);

    fclose(input);
    fclose(output);

    return 0;
}