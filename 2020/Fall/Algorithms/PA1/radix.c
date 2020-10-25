#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* obj;
    int start, len;
} Key;

int char_to_idx(char c) {
    if (c <= '9') return ((int) c - (int) '0');
    else if (c <= 'Z') return (((int) c - (int) 'A') + 10);
    return (((int) c - (int) 'a') + 10);
}

char check_valid_pos(Key* key, int idx, int d) {
    if (d < key[idx].len) return char_to_idx(key[idx].obj[key[idx].start + d]);
    return -1;
}

void radix_sort(Key* keys, Key* temp, int low, int high, int d) {
    if (high <= low) return;
    int bucket[38] = {0, }, cnt = high - low + 1;
    char idx;
    Key* dest, *src = &keys[low];

    for (int i = low; i <= high; i++) {
        idx = check_valid_pos(keys, i, d);
        bucket[idx + 2]++;
    }

    for (int i = 0; i < 37; i++) {
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

    for (int i = 0; i < 36; i++) {
        radix_sort(keys, temp, low + bucket[i], low + bucket[i + 1] - 1, d + 1);
    }
}

int main(int argc, char* argv[])
{
    //FILE* input = fopen("hw1_input.txt", "r");
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen("hw1_output.txt", "w");

    Key* keys, *temp; 
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
                keys[i].start = prev_key + 1;
                keys[i].len = len;
                break;
            }
        }
    }

    temp = (Key*) malloc(sizeof(Key) * n);
    radix_sort(keys, temp, 0, n - 1, 0);

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