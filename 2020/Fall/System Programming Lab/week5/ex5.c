#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main()
{
    while(1) {
        char* cmd = NULL;
        size_t size = 0;
        getline(&cmd, &size, stdin);
        cmd[strlen(cmd) - 1] = '\0';

        if (strcmp("quit", cmd) == 0) {
            wait(NULL);
            exit(0);
        }

        int cnt = 1;
        for (int i = 0; i < strlen(cmd) - 1; i++) {
            if (cmd[i] == ' ') cnt++;
        }
        char **arg = malloc(sizeof(char*) * cnt);

        int i = 0;
        char *ptr = strtok(cmd, " ");

        while (ptr != NULL) {
            arg[i++] = ptr;
            ptr = strtok(NULL, " ");
        }
        arg[i] = NULL;

        char path[100];
        sprintf(path, "/bin/%s", arg[0]);

        if (fork() == 0) {
            execv(path, arg);
        }
        wait(NULL);
        free(arg);
    }
}