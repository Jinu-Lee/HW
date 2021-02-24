#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n1_max 15000000
#define n2_max 150000000
#define hash_size 12251 //prime nearest to sqrt(n2_max)

int n1[n1_max];
int n2[n2_max];

int input[3];
int n1_num;
int n2_num;

int answer = 0; //최종 common

int hash_table[hash_size][(n2_max + hash_size - 1) / hash_size];

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common(){
    int i = 0;
    for(; i < n2_num; i++)
    {
        int row = n2[i] % hash_size;
        int col = ++hash_table[row][0];
        hash_table[row][col] = n2[i];
    }

    for(i = 0; i < n1_num; i++) {
        int row = n1[i] % hash_size;
        int j = 1, n = hash_table[row][0];
        for(; j <= n; j++) {
            if (n1[i] == hash_table[row][j]) {
                answer++;
                break;
            }
        }
    }
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
#elif 1
FILE *fp =  fopen("sample4_answer.txt", "r");
FILE *fp2 = fopen("sample4_n2.txt", "r");
FILE *fp3 = fopen("sample4_n1.txt", "r");
#elif 1 
FILE *fp =  fopen("sample5_answer.txt", "r");
FILE *fp2 = fopen("sample5_n2.txt", "r");
FILE *fp3 = fopen("sample5_n1.txt", "r");
#endif

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
