#include <stdio.h>
#define BUFSIZE	80

int GCD(int n, int m){
	int i, j;
	int ret;
	
	if(n < m) j = n;
	else j = m;

	for(i = j; i > 0; i--){
		if(n%i == 0 && m%i == 0) {
			ret = i;
			break;
		}
	}
	return ret;
}

int LCM(int n, int m){
	int i, j;
	int ret;
	
	if(n > m) j = n;
	else j = m;

	for(i = j ;; i+=j ){
		if(i%n == 0 && i%m == 0) {
			ret = i;
			break;
		}
	}
	return ret;
}

int main(void){
	int G, L;
    int n = 0;
    int m = 0;
	int cursor = 0;
	FILE *fd;
	char buffer[BUFSIZE];
	
	fd = fopen("pa1-1.input", "r");
	fread(buffer, sizeof(char), sizeof(buffer), fd);
	fclose(fd);

    while(buffer[cursor] != ' ' && buffer[cursor] != '\n'){
        n *= 10;
        n += buffer[cursor] - '0';
        cursor++;
    }
    cursor++;
    while(buffer[cursor] != ' ' && buffer[cursor] != '\n'){
        m *= 10;
        m += buffer[cursor] - '0';
        cursor++;
    }

    G = GCD(n, m);
    L = LCM(n, m);

    printf("GCD between %d and %d is %d\n", n, m, G);
	printf("LCM between %d and %d is %d\n", n, m, L);

	return 0;
}
