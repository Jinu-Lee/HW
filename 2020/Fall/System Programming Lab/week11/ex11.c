#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int row_size;
int col_size;

/*** Insert your own matrix & vector data, functions ***/
int** matrix;
int* vector;

void print_matrix(int**, int, int);
void print_vector(int*, int);
void generate_matrix(int**, int, int);
void generate_vector(int*, int);
/*** end ***/

typedef struct {
    int* row, *vec;
    int n, result;
} arg_struct;

void* thread(void* arg) {
    arg_struct* args = (arg_struct*)arg;
    int* row = args->row, * vec = args->vec;
    int n = args->n, result = 0;

    for (int i = 0; i < n; i++)
        result += row[i] * vec[i];

    args->result = result;
    pthread_exit(NULL);
}

int main(int argc, char* argv[ ]) {
    if (argc != 3) {
        printf("Usage: %s <row> <column>\n", argv[0]);
        exit(1);
    }

    row_size = atoi(argv[1]);
    col_size = atoi(argv[2]);

    matrix = malloc(sizeof(int*) * row_size);
    for (int i = 0; i < row_size; i++)
        matrix[i] = malloc(sizeof(int) * col_size);

    vector = malloc(sizeof(int) * col_size);

    arg_struct* args = malloc(sizeof(arg_struct) * row_size);
    pthread_t* tid = malloc(sizeof(pthread_t) * row_size);

    generate_matrix(matrix, row_size, col_size);
    generate_vector(vector, col_size);

    print_matrix(matrix, row_size, col_size);
    print_vector(vector, col_size);

    for (int i = 0; i < row_size; i++) {
        args[i].row = matrix[i];
        args[i].vec = vector;
        args[i].n = col_size;
        if (pthread_create(&tid[i], NULL, thread, (void*)&args[i])) {
            printf("ERROR: pthread creation failed.\n");
            exit(1);
        }
    }

    for (int i = 0; i < row_size; i++)
        pthread_join(tid[i], NULL);

    printf(" *** Result ***\n");
    for (int i = 0; i < row_size; i++) {
        printf("[ %d ]\n", args[i].result);
    }

    for (int i = 0; i < row_size; i++)
        free(matrix[i]);
    free(matrix);
    free(vector);
    free(args);
    free(tid);

    return 0;
}

/***  skeleton code ***/
void print_matrix(int** mat, int rows, int cols) {
    printf(" *** Matrix ***\n");
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("[ %d ] ", mat[r][c]);
        }
        printf("\n");
    }
}
void print_vector(int* vec, int size) {
    printf(" *** Vector ***\n");
    for (int i = 0; i < size; i++) {
        printf("[ %d ]\n", vec[i]);
    }
}

void generate_matrix(int** mat, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            mat[r][c] = rand() % 10;
        }
    }
}
void generate_vector(int* vec, int size) {
    for (int i = 0; i < size; i++) {
        vec[i] = rand() % 10;
    }
}
/*** end ***/