#include <stdio.h>
#include <string.h>

long long num2eng(char* input) {
    long long memo[2001] = {1, 1, };
    int i = 1;
    if (input[0] == '0') {
        return 0;
    }

    for (; input[i] != '\0'; i++) {
        if (input[i] == '0') {
            if (input[i - 1] == '1' || input[i - 1] == '2') {
                memo[i + 1] = memo[i - 1];
            }
            else {
                return 0;
            }
        }
        else if (input[i - 1] == '1' || (input[i - 1] == '2' && input[i] <= '6' && input[i] >= '1')) {
            memo[i + 1] = memo[i] + memo[i - 1];
        }
        else {
            memo[i + 1] = memo[i];
        }
    }

    return memo[i];
}

int main() {
    char input[2000];
    scanf("%s", input);
    long long answer;
    for(int i = 0; i < 300000; i++) 
        answer = num2eng(input);
    printf("%lld", answer);
    return 0;
}