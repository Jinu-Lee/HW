#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUMBER 500
 
void main() 
{        
    int i, tmp;
    int save[NUMBER];
    int count = 0;
    int isSame = 0;
    srand(time(0));

    FILE *f = fopen("input.txt", "w");
    
    //숫자 추출
    while (count < NUMBER) {
        isSame = 0;
        tmp = (rand() * rand()) % 500;
        if(rand() % 2 != 0)
            tmp = -tmp;
        for (int i = 0; i < count; i++) { //중복검사
            if (tmp == save[i]) { //중복이 있을때
                isSame = 1;
                break;
            }
        }
        if (isSame == 0) { //중복없음
            save[count] = tmp;
            count++;
        }
    }
 
    for(i =0; i < NUMBER; i++)    
        fprintf(f, "%d ", save[i]);
 
}
