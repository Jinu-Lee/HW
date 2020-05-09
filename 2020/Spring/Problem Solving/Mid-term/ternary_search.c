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

int ternary_search(int l, int h, int k)
{
    if(l == h - 1) {
		if(k == n2[l] || k == n2[h])
			return 1;
		else
			return 0;
	}
	if(l == h - 2) {
		if(k == n2[l] || k == n2[l + 1] || k == n2[h])
			return 1;
		else
			return 0;
	}
    if(l <= h)
    {
        int intvl = (h - l) / 3;
        int l_m = l + intvl;
        int r_m = l_m + intvl;
        if (n2[l_m] == k) return 1;
        if (n2[r_m] == k) return 1;
        if (k < n2[l_m]) return ternary_search(l, l_m, k);
        else if (k > n2[l_m] && k < n2[r_m]) return ternary_search(l_m, r_m, k);
        else return ternary_search(r_m, h, k);
    }
    return 0;
}

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common()
{
	for (int i = 0; i < n1_num; i++)
		if (ternary_search(0, n2_num - 1, n1[i]))
			answer++;
}

int main()
{

FILE *fp =  fopen("sample5_answer.txt", "r");
FILE *fp2 = fopen("sample5_n2.txt", "r");
FILE *fp3 = fopen("sample5_n1.txt", "r");
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
