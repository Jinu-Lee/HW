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

int answer = 0; //final common

void jump_linear(){
    int jmp = 1, cnt = 0, i = 0, j = 0;
    while(i < n2_num && j < n1_num) {
        if(n2[i] < n1[j])
        {   
            cnt++;
            if(cnt >= 4) {
                jmp += 10;
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

void normal_linear()
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
            answer++;
            j++;
            i++;
        }
    }
}

// Find common part of two array and reserve at global variable 'answer' 
void find_common(){
	if (n2_num < n1_num * 2) {
        normal_linear();
	}
	else {
		jump_linear();
	}
}

int main()
{

FILE *fp  = fopen("sample5_answer.txt", "r");
FILE *fp2 = fopen("sample5_n2.txt", "r");
FILE *fp3 = fopen("sample5_n1.txt", "r");

for(int i =0; i<3; i++){
        fscanf(fp, "%d", &input[i]);
}

fclose(fp);

n2_num = input[0];
n1_num = input[1];
int common = input[2]; //Correct Answer!

for(int i =0; i<n2_num; i++){
        fscanf(fp2, "%d", &n2[i]);
}

for(int i =0; i<n1_num; i++){
        fscanf(fp3, "%d", &n1[i]);
}


fclose(fp2);
fclose(fp3);

find_common();

printf("Real_Common : %d\n",common ); //Real answer
printf("Calculated_Common : %d\n",answer ); //Your guess


return 0;

}
