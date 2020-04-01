#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
} Node;

Node *head, *tail;

Node *newNode(int x)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = x;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

void Insert(int x) //insert at tail
{
    Node *new = newNode(x);

    if (head == NULL && tail == NULL)
    {
        head = tail = new;
    }
    else
    {
        tail->next = new;
        new->prev = tail;
        tail = new;
    }
}

void Delete(Node *node)
{
    if (head == NULL && tail == NULL)
        return;

    if (node == head)
    {
        head = head->next;
        if(head!=NULL) head->prev = NULL;
    }
    else if (node == tail)
    {
        tail = tail->prev;
        if(tail!=NULL) tail->next = NULL;
    }
    else
    {
        Node *prevNode = node->prev;
        Node *nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }
    free(node);
}

Node *compare(Node *node) //compare 후 없앨 index를 반환
{
    int sign = (node->data > 0) ? 1 : -1;
    Node *temp = (sign > 0) ? node->next : node->prev;

    if (node->data == 0 || temp == NULL) //skip
        return NULL;

    if ((node->data) * (temp->data) > 0) //양쪽 숫자 부호가 같으면 충돌 X
        return NULL;
    else
    {
        //비교할 대상과 sign의 부호 비교
        int sum = (node->data) + (temp->data);

        if (sum == 0) //더한 값 = 0 -> 둘 다 사라짐
        {
            Delete(node); //한쪽 없애고 한쪽 return
            return temp;
        }

        if ((sum ^ sign) >> 31 == 0) //더한 값과 원래 값의 부호가 다름
            return temp;
        else //더한 값과 원래 값의 부호가 같음
            return node;
    }
}

int *RunawayTrain(int n, int *trains)
{
    //You should return this result array. If 0 comes, we will consider your answer is end.
    int *result = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
        Insert(trains[i]);

    int cnt;
    Node *node;
    while (1)
    {
        cnt = 0;
        for (node = head; node != NULL; node = node->next)
        {
            Node *temp = compare(node);

            if (temp != NULL)
            {
                Delete(temp);
                cnt++;
            }
        }
        if (cnt == 0) //cnt = 0 -> 변함 X -> 충돌 끝
            break;
    }

    cnt = 0;
    for (node = head; node != NULL; node = node->next)
        if (node->data != 0)
            result[cnt++] = node->data; //0이 아닌 수들을 옮김
    result[cnt] = 0;                    //마지막 수를 0으로 설정

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