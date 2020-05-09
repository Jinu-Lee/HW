#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n1_max 15000000
#define n2_max 150000000
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int n1[n1_max];
int n2[n2_max];

int input[3];
int n1_num;
int n2_num;

int answer = 0; //최종 common

int binary_search(int l, int h, int k)
{
	int m = (l + h) / 2;
	while (l <= h && n2[m] != k)
	{
		if (n2[m] > k) h = m - 1;
		else l = m + 1;
        m = (l + h) / 2;
	}
    if(n2[m] == k) return 1;
	return 0;
}

int exponential_search(int k)
{
	if (n2[0] == k) return 1;
	int i = 1;
	while(i < n2_num && n2[i] <= k)
		i *= 2;
	return binary_search(i / 2, MIN(i, n2_num - 1), k);
}

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common()
{
	for (int i = 0; i < n1_num; i++)
		if (exponential_search(n1[i]))
			answer++;
}

int main()
{

#if 1
FILE *fp =  fopen("sample1_answer.txt", "r");
FILE *fp2 = fopen("sample1_n2.txt", "r");
FILE *fp3 = fopen("sample1_n1.txt", "r");
#elif 1
FILE *fp =  fopen("sample2_answer.txt", "r");
FILE *fp2 = fopen("sample2_n2.txt", "r");
FILE *fp3 = fopen("sample2_n1.txt", "r");
#elif 1
FILE *fp =  fopen("sample3_answer.txt", "r");
FILE *fp2 = fopen("sample3_n2.txt", "r");
FILE *fp3 = fopen("sample3_n1.txt", "r");
#elif 1
FILE *fp =  fopen("sample4_answer.txt", "r");
FILE *fp2 = fopen("sample4_n2.txt", "r");
FILE *fp3 = fopen("sample4_n1.txt", "r");
#elif 1
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
