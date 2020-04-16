#include <stdio.h>
#include <stdlib.h>

int N;

int compare(int idx, int *trains)                       //폭발할 기차의 index를 반환하는 함수
{
    if (trains[idx] == 0)                               //0은 기차가 없는 상태 -> skip
        return -1;

    int dir = (trains[idx] > 0) ? 1 : -1;               //기차가 +면 오른쪽으로 이동 -> 오른쪽 기차와 비교, -면 왼쪽으로 이동 -> 왼쪽 기차와 비교
    int pos = dir;                                      //비교 대상의 상대적인 위치

    while (1)
        if (idx + pos >= N || idx + pos < 0)            //out of index
            return -1;
        else if (trains[idx + pos] != 0)                //0이 아닌 기차를 찾으면 break
            break;
        else
            pos += dir;                                 //1칸 옮김

    if ((trains[idx] ^ trains[idx + pos]) >> 31 == 0)   //양쪽 기차의 부호가 같으면 충돌 X
        return -1;
    else
    {
        int sum = trains[idx] + trains[idx + pos];      

        if (sum == 0)                                   //더한 값 = 0 -> 둘 다 폭발
        {
            trains[idx] = 0;                            //한쪽 없애고 한쪽 return
            return idx + pos;
        }

        if ((sum ^ dir) >> 31 == 0)                     //더한 값과 원래 값의 부호가 같음 -> 비교 대상이 폭발
            return idx + pos;
        else                                            //더한 값과 원래 값의 부호가 다름 -> 자신이 폭발
            return idx;
    }
}

int *RunawayTrain(int n, int *trains)
{
    //You should return this result array. If 0 comes, we will consider your answer is end.
    int *result = (int *)malloc(sizeof(int) * n);
    N = n;

    int cnt, i;
    while (1)
    {
        cnt = 0;
        for (i = 0; i < n; i++)
        {
            int idx = compare(i, trains);

            if (idx != -1)
            {
                trains[idx] = 0;                        //폭발할 기차의 값을 0으로 assign
                cnt++;                                  //폭발 횟수 저장
            }
        }
        if (cnt == 0)                                   //cnt = 0 -> 폭발 X -> 끝
            break;
    }

    cnt = 0;
    for (i = 0; i < n; i++)
        if (trains[i] != 0)
            result[cnt++] = trains[i];                  //남아있는 기차를 결과에 복사
    result[cnt] = 0;                                    //마지막 값을 0으로 설정

    return result;
}

int main()
{
    int n;
    scanf("%d", &n);

    int *trains = (int *)malloc(sizeof(int) * n);
    int i = 0;
    for (i = 0; i < n; i++)
        scanf("%d", &trains[i]);

    int *result = (int *)malloc(sizeof(int) * n);

    result = RunawayTrain(n, trains);

    for (i = 0; i < n; i++)
    {
        if (result[i] != 0)
            printf("%d\n", result[i]);
        else
            break;
    }

    free(trains);

    return 0;
}