#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1001

#define DIAGONAL 0b1
#define LEFT 0b10
#define UP 0b100

char DNA[5][MAX_LEN];
int n;

void find_lcs(char[], char[], int, int, int[][MAX_LEN], int[][MAX_LEN]);
void get_lcs(char[], char[], int[][MAX_LEN], char[MAX_LEN], char[MAX_LEN]);
void insert_gap(char[], char[], char[], char[]);
int max(int, int, int, int*);

int main(int argc, char* argv[])
{
    char input_default[] = "hw2_input.txt";
    char output_default[] = "hw2_output.txt";
    char* input_file = input_default, *output_file = output_default;

    char center_aligned[4][MAX_LEN], other_aligned[4][MAX_LEN], temp[6][MAX_LEN], answer[6][MAX_LEN];
    int score[MAX_LEN][MAX_LEN], move[MAX_LEN][MAX_LEN], i, j, k, max_star = -1, max_center;

    if (argc >= 2) input_file = argv[1];
    if (argc == 3) output_file = argv[2];

    FILE* input = fopen(input_file, "r");
    FILE* output = fopen(output_file, "w");

    fscanf(input, "%d ", &n);
    fgetc(input); fgetc(input);

    for (i = 0; i < n; i++) {
        fscanf(input, "%s", DNA[i]);
    }

    for (i = 0; i < n; i++) {
        for (j = 0, k = 0; j < n; j++) {
            if (i == j) continue;
            
            find_lcs(DNA[i], DNA[j], 3, 1, score, move);
            get_lcs(DNA[i], DNA[j], move, center_aligned[k], other_aligned[k]);
            k++;
        }

        for (j = 0; j < 2; j++) {
            for (k = 1; k < n - 1; k++) {
                insert_gap(center_aligned[0], center_aligned[k], other_aligned[0], other_aligned[k]);
            }
        }

        strcpy(temp[0], center_aligned[0]);
        for (j = 0; j < n - 1; j++) {
            strcpy(temp[j + 1], other_aligned[j]);
        }

        k = strlen(center_aligned[0]); 
        int cnt = 0;
        for (j = 0; j < k; j++) {
            int flag = 1;
            for (int l = 0; l < n - 1; l++) {
                flag = flag & (temp[l][j] == temp[l + 1][j]);
            }
            if (flag == 1) {
                temp[n][j] = '*';
                cnt++;
            }
            else temp[n][j] = ' ';
        }
        temp[n][k] = '\0';

        if (cnt > max_star) {
            for (j = 0; j < n + 1; j++) {
                strcpy(answer[j], temp[j]);
            }
            max_star = cnt;
            max_center = i;
        }
    }

    for (i = 0; i < max_center; i++) {
        char temp[MAX_LEN];

        strcpy(temp, answer[i]);
        strcpy(answer[i], answer[i + 1]);
        strcpy(answer[i + 1], temp);
    }

    for (i = 0; i < n + 1; i++) {
        fprintf(output, "%s", answer[i]);
        if (i != n) fputc('\n', output);
    }

    fclose(input);
    fclose(output);

    return 0;
}

void find_lcs(char x[], char y[], int right, int wrong, int score[][MAX_LEN], int move[][MAX_LEN]) {
    int m = strlen(x), n = strlen(y), flag;

    for (int i = 0; i <= m; i++) score[i][0] = 0;
    for (int j = 1; j <= n; j++) score[0][j] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            score[i][j] = max(score[i - 1][j - 1] + right * (x[i - 1] == y[j - 1]), score[i][j - 1] + wrong, score[i - 1][j] + wrong, &flag);
            move[i][j] = (1 << flag);
        }
    }
}

void get_lcs(char x[], char y[], int move[][MAX_LEN], char s1[MAX_LEN], char s2[MAX_LEN]) {
    char x_temp[MAX_LEN], y_temp[MAX_LEN];
    int m = strlen(x), n = strlen(y);
    int i, j, l = m + n, x_pos = l, y_pos = l;

    for (i = m, j = n; (i != 0) && (j != 0);) {
        if (move[i][j] == DIAGONAL) {
            x_temp[x_pos--] = x[--i];
            y_temp[y_pos--] = y[--j];
        }
        else if (move[i][j] == LEFT) {
            x_temp[x_pos--] = '-';
            y_temp[y_pos--] = y[--j];
        } 
        else {
            x_temp[x_pos--] = x[--i];
            y_temp[y_pos--] = '-';
        }
    }

    while (x_pos) {
        if (i > 0) x_temp[x_pos--] = x[--i];
        else x_temp[x_pos--] = '-';
    }
    while (y_pos) {
        if (j > 0) y_temp[y_pos--] = y[--j];
        else y_temp[y_pos--] = '-';
    }

    for (i = l, j = 1; i > 0; i--) {
        if (y_temp[i] == '-' && x_temp[i] == '-') {
            j = i + 1; 
            break;
        }
    }

    x_temp[l + 1] = '\0';
    y_temp[l + 1] = '\0';
    strcpy(s1, x_temp + j);
    strcpy(s2, y_temp + j);
}

void insert_gap(char center_x[], char center_y[], char other_x[], char other_y[]) {
    int m, n, x_pos, y_pos, ans_pos, other1_pos, other2_pos;
    char center[MAX_LEN], other1[MAX_LEN], other2[MAX_LEN];

    m = strlen(center_x), n = strlen(center_y);
    x_pos = 0, y_pos = 0, ans_pos = 0, other1_pos = 0, other2_pos = 0;

    while ((x_pos < m) || (y_pos < n)) {
        if (center_x[x_pos] == center_y[y_pos]) {
            center[ans_pos++] = center_x[x_pos];
            other1[other1_pos++] = other_x[x_pos++];
            other2[other2_pos++] = other_y[y_pos++];
        } else if (center_x[x_pos] == '-') {
            center[ans_pos++] = '-';
            other1[other1_pos++] = other_x[x_pos++];
            other2[other2_pos++] = '-';
        } else {
            center[ans_pos++] = '-';
            other1[other1_pos++] = '-';
            other2[other2_pos++] = other_y[y_pos++];
        }
    }

    center[ans_pos] = '\0';
    other1[other1_pos] = '\0';
    other2[other2_pos] = '\0';

    strcpy(center_x, center);
    strcpy(center_y, center);
    strcpy(other_x, other1);
    strcpy(other_y, other2);
}

int max(int a, int b, int c, int* flag) {
    int max = a, temp = 0;

    if (b > max) {
        max = b;
        temp = 1;
    }
    if (c > max) {
        max = c;
        temp = 2;
    }

    *flag = temp;
    return max;
}