#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

char** board[2];
int m, N, n_threads, cnt;
pthread_mutex_t* lock;

void* thread(void* arg) {
    char** curr_board = board[cnt % 2];
    char** next_board = board[1 - (cnt % 2)];
    char* row_start, *row_end, *curr, *next;
    int row = ((int) arg), n_live, next_state;
    int* sum = calloc(m + 2, sizeof(int));

    row_start = curr_board[row - 1];
    row_end = row_start + m + 1;
    for (curr = row_start; curr < row_end; curr++)
        sum[curr - row_start] += *curr;        
    
    row_start = curr_board[row];
    row_end = row_start + m + 1;
    for (curr = row_start; curr < row_end; curr++)
        sum[curr - row_start] += *curr;
    
    row_start = curr_board[row + 1];
    row_end = row_start + m + 1;
    for (curr = row_start; curr < row_end; curr++)
        sum[curr - row_start] += *curr;

    curr = curr_board[row];
    next = next_board[row];

    pthread_mutex_lock(&lock[row - 1]);

    for (int i = 1; i <= m; i++) {
        n_live = sum[i - 1] + sum[i] + sum[i + 1];
        if (curr[i]) 
            next_state = (n_live == 3 || n_live == 4);
        else 
            next_state = (n_live == 3);
        next[i] = next_state;
    }

    pthread_mutex_unlock(&lock[row - 1]);
    free(sum);

    pthread_exit(NULL);
}

int main()
{
    int* args;
    pthread_t* tid;

    scanf("%d", &m);
    scanf("%d", &N);
    scanf("%d", &n_threads);

    tid = (pthread_t*) malloc(sizeof(pthread_t) * n_threads);
    lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * m);
    board[0] = (char**) malloc(sizeof(char*) * (m + 2));
    board[1] = (char**) malloc(sizeof(char*) * (m + 2));

    for (int i = 0; i < 2; i++) {
        board[0][i] = (char*) calloc(m + 2, sizeof(char));
        board[1][i] = (char*) calloc(m + 2, sizeof(char));
    }

    for (int i = 0; i < m; i++) {
        board[0][i + 2] = (char*) calloc(m + 2, sizeof(char));
        board[1][i + 2] = (char*) calloc(m + 2, sizeof(char));
        pthread_mutex_init(&lock[i], (pthread_mutexattr_t*) NULL);

        for (int j = 1; j <= m; j++)
            scanf("%1hhd", &board[0][i + 1][j]);
    }

    for (cnt = 0; cnt < N; cnt++) {
        for (int i = 1; i <= m; i += n_threads) {
            for (int j = 0; j < n_threads; j++) {
                if (i + j > m) break;

                if (pthread_create(&tid[j], NULL, thread, (void*) (i + j))) {
                    printf("pthread create() failed.\n");
                    exit(1);
                }
            }
            
            for (int j = 0; j < n_threads; j++) {
                if (i + j > m) break;
                pthread_join(tid[j], NULL);
            }
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) 
            printf("%hhd", board[N % 2][i][j]);
        printf("\n");
    }

    for (int i = 0; i < m + 2; i++) {
        free(board[0][i]);
        free(board[1][i]);
    }
    free(board[0]);
    free(board[1]);
    free(tid);
    free(lock);
    
    return 0;
}