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

int cnt[3];

// 두 배열의 공통 부분을 찾아 전역변수 answer에 저장하시오. 함수를 반환형을 바꾸어 return을 사용하셔도 됩니다.
void find_common(){
    int jmp1 = 1, jmp2 = 1, cnt1 = 0, cnt2 = 0, i = 0, j = 0;
    while (i < n2_num && j < n1_num) {
        if(n2[i] < n1[j])
        {   
            cnt2 = 0;
            if(jmp2 > 1) {
                j = j - jmp2 + 1;
                jmp2 = 1;
                continue;
            }
            cnt1++;
            if(cnt1 == 2) {
                jmp1 = 2;    
            }
            i += jmp1;
            if(i > n2_num) {
                i = i - jmp1 + 1;
                jmp1 = 1;
            }
        }
        else if(n2[i] > n1[j])
        {
            cnt1 = 0;
            if(jmp1 > 1) {
                i = i - jmp1 + 1;
                jmp1 = 1;
                continue;
            }
            cnt2++;
            if(cnt2 == 2) {
                jmp2 = 2;    
            }
            j += jmp2;
            if (j > n1_num) {
                j = j - jmp2 + 1;
                jmp2 = 1;
            }
        }
        else 
        {
            cnt1 = cnt2 = 0; jmp1 = jmp2 = 1;
            answer++;
            i++; j++;
        }
    }
}


int main()
{
FILE *fp  = fopen("testcase/ratio_100/t8_answer.txt", "r");
FILE *fp2 = fopen("testcase/ratio_100/t8_n2.txt", "r");
FILE *fp3 = fopen("testcase/ratio_100/t8_n1.txt", "r");

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
