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

void double_binary_search(int n1_l, int n1_r, int n2_l, int n2_r)
{
	if(n1_l > n1_r || n2_l > n2_r)
		return;

	int flag = 0;
	int n1_m = (n1_l + n1_r) / 2;
	int n2_m = binary_search(n2_l, n2_r, n1[n1_m], &flag);
	
	answer += flag;

	double_binary_search(n1_l, n1_m - 1, n2_l, n2_m - flag);
	double_binary_search(n1_m + 1, n1_r, n2_m + flag, n2_r);
}

void linear_search()
{
	for(int i = 0, j = 0; i < n2_num, j < n1_num;) {
        int temp = n2[i] - n1[j];
        if(temp < 0)
        {
            i++;
        }
        else if(temp == 0)
        {
			answer++;
            j++;
			i++;
        }
        else
        {
            j++;
        }
	}
}

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common()
{
	if(n2_num < (n1_num << 1))
		linear_search();
    else
		double_binary_search(0, n1_num - 1, 0, n2_num - 1);
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
