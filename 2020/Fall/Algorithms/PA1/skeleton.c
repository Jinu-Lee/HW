#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* word;
    int idx;
} Key;

int compare(char* w1, char* w2) {
    int i;
    for (i = 0; (w1[i] == w2[i]) && w2[i] != '\0'; i++) {}

    char l1 = w1[i], l2 = w2[i];
    if (l1 < l2) 
        return -1;
    else if (l1 > l2)
        return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    //FILE* input = fopen("hw1_input.txt", "r");
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen("hw1_output.txt", "w");

    Key* keys; 
    char** obj, buf[165];
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
    obj = (char**) malloc(sizeof(char*) * n);

    //read object one by one
    fgetc(input); fgetc(input);
    for (int i = 0, idx = 0; i < n; i++) {
        int size = 16 * t + 1, prev_key, curr_key = -1, cnt = 0;
        char* buf = (char*) malloc(size);
        obj[i] = buf;
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
                char* word1 = (char*) malloc(sizeof(char) * len);
                char* word2 = buf + prev_key + 1;
                for (int i = 0; i < len; i++) {
                    word1[i] = word2[i];
                }
                keys[idx].idx = i;
                keys[idx++].word = word1;
                break;
            }
        }
    }

    /* sort */

    for (int i = 0; i < n; i++) {
        fputs(obj[keys[i].idx], output);
        if (i != n - 1) {
            fputc('\n', output);
        }
    }

    for (int i = 0; i < n; i++) {
        free(keys[i].word);
    }
    free(keys);

    fclose(input);
    fclose(output);

    return 0;
}