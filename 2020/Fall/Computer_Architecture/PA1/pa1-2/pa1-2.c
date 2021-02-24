# include <stdio.h>
# define MAX 10
# define BUF_SIZE 1000

int A[MAX][MAX] = {0};
int B[MAX][MAX] = {0};
int C[MAX][MAX] = {0};
int DIM = 0;
int N = 0;

void product(int n){
    int i, j, k;
    if(n == 1) return;
    for(i = 0; i < DIM; i++){
        for(j = 0; j < DIM; j++){
            for(k = 0; k < DIM; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for(i = 0; i < DIM; i++){
        for(j = 0; j < DIM; j++){
            B[i][j] = C[i][j];
            C[i][j] = 0;
        }
    }
    return product(n-1);
}

void printResult(){
    int i, j;
    for(i = 0; i < DIM; i++){
        for(j = 0; j < DIM; j++){
            printf("%d\t" ,B[i][j]);
        }
        printf("\n");
    }
}
void main(){
    int i,j;
    FILE *fd;
    int cursor = 0;
    int v = 0;
    char buffer[BUF_SIZE];

    fd = fopen("pa1-2.input", "r");
	fread(buffer, sizeof(char), sizeof(buffer), fd);
	fclose(fd);

    while(buffer[cursor] != ' ' && buffer[cursor] != '\n'){
		DIM *= 10;
		DIM += buffer[cursor] - '0';
		cursor++;
    }
    cursor++;
    while(buffer[cursor] != ' ' && buffer[cursor] != '\n'){
		N *= 10;
		N += buffer[cursor] - '0';
		cursor++;
    }
    cursor++;
    for(i = 0; i < DIM; i++){
        for(j = 0; j < DIM; j++){
            while(buffer[cursor] != ' ' && buffer[cursor] != '\n'){
                v *= 10;
                v += buffer[cursor] - '0';
                cursor++;
            }
            A[i][j] = v;
            B[i][j] = v;
            v=0;
            cursor++;
        }
    }
    product(N);
    printResult();
}
