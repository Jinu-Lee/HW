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

int binary_search(int k)
{
	int l = 0, h = n2_num - 1;
	while (l <= h)
	{
		int m = (l + h) / 2;
		if (n2[l] > k || n2[h] < k)
			return 0;
		if (n2[l] == k || n2[h] == k || n2[m] == k)
			return 1;
		if (n2[m] > k)
		{
			h = m - 1;
			l++;
		}
		else if (n2[m] < k)
		{
			l = m + 1;
			h--;
		}
	}
	return 0;
}

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common()
{
	for (int i = 0; i < n1_num; i++)
		if (binary_search(n1[i]))
			answer++;
}

int main()
{

FILE *fp =  fopen("sample1_answer.txt", "r");
FILE *fp2 = fopen("sample1_n2.txt", "r");
FILE *fp3 = fopen("sample1_n1.txt", "r");


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
