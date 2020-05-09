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

int answer = 0; //최종 common

int n1_L[30];
int n1_R[30];
int n2_L[30];
int n2_R[30];
int top;

int binary_search(int l, int r, int k, int* flag)
{
	int m;
	while (l <= r)
	{
		m = (l + r) / 2;
		if (n2[m] == k) {
			*flag = 1;
			break;
		}
		if (n2[m] > k) r = m - 1;
		else l = m + 1;
	}
	return m;
}

void double_binary_search()
{
    int flag, n1_m, n2_m, t1, t2, t3, t4;
	while(top != -1) {
        t1 = n1_L[top]; t2 = n1_R[top]; t3 = n2_L[top]; t4 = n2_R[top];
        n1_L[top--] = (n1_R[top] = n2_L[top] = n2_R[top] = 0);

        if(t1 > t2 || t3 > t4) {
            continue;
        }
        flag = 0;
        n1_m = (t1 + t2) / 2;
        n2_m = binary_search(t3, t4, n1[n1_m], &flag);

        answer += flag;

        n1_L[++top] = t1; n1_R[top] = n1_m - 1; n2_L[top] = t3; n2_R[top] = n2_m - flag;
        n1_L[++top] = n1_m + 1; n1_R[top] = t2; n2_L[top] = n2_m + flag; n2_R[top] = t4;
    }
}

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common()
{
    n1_L[0] = 0; n1_R[0] = n1_num - 1; n2_L[0] = 0; n2_R[0] = n2_num - 1;
	double_binary_search();
}

int main()
{

#if 0
FILE *fp =  fopen("sample1_answer.txt", "r");
FILE *fp2 = fopen("sample1_n2.txt", "r");
FILE *fp3 = fopen("sample1_n1.txt", "r");
#elif 0
FILE *fp =  fopen("sample2_answer.txt", "r");
FILE *fp2 = fopen("sample2_n2.txt", "r");
FILE *fp3 = fopen("sample2_n1.txt", "r");
#elif 0
FILE *fp =  fopen("sample3_answer.txt", "r");
FILE *fp2 = fopen("sample3_n2.txt", "r");
FILE *fp3 = fopen("sample3_n1.txt", "r");
#elif 0
FILE *fp =  fopen("sample4_answer.txt", "r");
FILE *fp2 = fopen("sample4_n2.txt", "r");
FILE *fp3 = fopen("sample4_n1.txt", "r");
#else
FILE *fp =  fopen("sample5_answer.txt", "r");
FILE *fp2 = fopen("sample5_n2.txt", "r");
FILE *fp3 = fopen("sample5_n1.txt", "r");
#endif
/*
FILE *fp =  fopen("ans.txt", "r");
FILE *fp2 = fopen("n2.txt", "r");
FILE *fp3 = fopen("n1.txt", "r");
*/

for(int i =0; i<3; i++){
        fscanf(fp, "%d", &input[i]);
}

fclose(fp);

n2_num = input[0];
n1_num = input[1];
int common = input[2]; //정답!

for(int i =0; i<n2_num; i++){
        fscanf(fp2, "%d", &n2[i]);
}

for(int i =0; i<n1_num; i++){
        fscanf(fp3, "%d", &n1[i]);
}


fclose(fp2);
fclose(fp3);

find_common();

printf("Real_Common : %d\n",common ); //실제 정답 출력
printf("Calculated_Common : %d\n",answer ); //찾은 정답 출력


return 0;

}
