#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define R 20000

int points_in_circle, points_total, n, m;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int inCircle(int x, int y) {
    return (x * x + y * y <= R * R);
}

void* thread() {
    int x, y;
    for (int i = 0; i < m; i++) {
        pthread_mutex_lock(&lock);

        x = rand() % (R + 1);
        y = rand() % (R + 1);
        if (inCircle(x, y)) points_in_circle++;

        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[])
{
    n = atoi(argv[1]), m = atoi(argv[2]);

    pthread_t* tid = malloc(sizeof(pthread_t) * n);

    for (int i = 0; i < n; i++) {
        if (pthread_create(&tid[i], NULL, thread, NULL)) {
            printf("ERROR: pthread creation failed.\n");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++)
        pthread_join(tid[i], NULL);

    points_total = n * m;
    float pi = 4.0 * points_in_circle / points_total;
    printf("PI = %.10f\n", pi);

    free(tid);
    return 0;
}