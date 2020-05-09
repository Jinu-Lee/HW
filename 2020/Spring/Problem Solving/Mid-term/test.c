//gcc test.c -o test && ./test
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n1_max 15000000
#define n2_max 150000000

int n1[n1_max];
int n2[n2_max];

int input[3];
int n1_num;
int n2_num;

int answer = 0, answer2 = 0, answer3 = 0; //최종 common

void find_common(){
    int jmp = 1, cnt = 0, i = 0, j = 0;
    while(i < n2_num && j < n1_num) {
        if(n2[i] < n1[j])
        {   
            cnt++;
            if(cnt >= 4) {      //변수 1
                jmp += 10;      //변수 2
            }
            i += jmp;
            if(i > n2_num) {
                i = i - jmp + 1;
                jmp = 1;
            }
        }
        else if(n2[i] > n1[j])
        {
            cnt = 0;
            if (jmp > 1)
            {
                i = i - jmp + 1;
                jmp = 1;
                continue;
            }
            j++;
        }
        else
        {
            cnt = 0; jmp = 1;
            answer++;
            i++; j++;
        }
    }
}

void find_common_linear()
{
    int i = 0, j = 0;
    while(i < n2_num && j < n1_num)
    {
        if (n2[i] < n1[j])
        {
            i++;
        }
        else if (n2[i] > n1[j])
        {
            j++;
        }
        else
        {
            answer2++;
            j++;
            i++;
        }
    }
}

int main()
{

    FILE *fp  = fopen("testcase/sample/sample1_answer.txt", "r");
    FILE *fp2 = fopen("testcase/sample/sample1_n2.txt", "r");
    FILE *fp3 = fopen("testcase/sample/sample1_n1.txt", "r");
    for (int i = 0; i < 3; i++)
    {
        fscanf(fp, "%d", &input[i]);
    }

    fclose(fp);

    n2_num = input[0];
    n1_num = input[1];
    int common = input[2]; //정답!

    for (int i = 0; i < n2_num; i++)
    {
        fscanf(fp2, "%d", &n2[i]);
    }

    for (int i = 0; i < n1_num; i++)
    {
        fscanf(fp3, "%d", &n1[i]);
    }

    fclose(fp2);
    fclose(fp3);

    struct timespec begin, end;
    double temp = 0, temp2 = 0;
    for (int i = 1; i <= 100; i++)
    {
        answer = 0;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        find_common();
        clock_gettime(CLOCK_MONOTONIC, &end);
        temp += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
        answer2 = 0;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        find_common_linear();
        clock_gettime(CLOCK_MONOTONIC, &end);
        temp2 += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    }
    printf("normal linear: %.6f\t", temp2 / 100.0);
    printf("\tn2/n1: %.6f\n", (float) n2_num/n1_num);
    printf("custom linear: %.6f\t", temp / 100.0);
    printf("\tratio: %.6f\n", temp2 / temp);
    printf("\ncustom lin: %d\tnormal lin: %d\tanswer: %d\n",answer, answer2, common);

    return 0;
}
