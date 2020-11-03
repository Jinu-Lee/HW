#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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
            if (cmd[i] == ' ')
                cnt++;
        }
        char *arg[100];

        char* search1 = strchr(cmd, '>');
        char* search2 = strchr(cmd, '<');
        char* search3 = strchr(cmd, '|');

        int i = 0;
        char *ptr = strtok(cmd, " ");

        while (ptr != NULL) {
            arg[i++] = ptr;
            ptr = strtok(NULL, " ");
        }
        arg[i] = NULL;

        char path[100];
        sprintf(path, "/bin/%s", arg[0]);
        
        pid_t pid;
        if (search1 != NULL || search2 != NULL) {      // '>'
            int fd1, fd2, flag;
            flag = (search1) ? O_CREAT | O_WRONLY | O_TRUNC : O_RDONLY;
            fd2 = (search1) ? STDOUT_FILENO : STDIN_FILENO;

            if ((fd1 = open(arg[cnt - 1], flag, 0755)) < 0) {
                printf("error");
                exit(1);
            }

            arg[cnt - 1] = NULL;
            arg[cnt - 2] = NULL;

            if ((pid = fork()) == 0) { // child
                dup2(fd1, fd2);
                execv(path, arg);
            }
            wait(NULL);

            if (close(fd1) < 0) {
                printf("error");
                exit(1);
            }
        }
        else if (search3 != NULL) { // '|'
            int fd[2];
            pipe(fd);

            for (i = 0; i < cnt; i++) 
                if (strcmp(arg[i], "|") == 0) break;

            char path2[100], *arg2[100];
            sprintf(path2, "/bin/%s", arg[i + 1]);
            
            for (int j = i + 1; j < cnt; j++) {
                strcpy(arg2[j - i - 1], arg[j]);
                arg[j] = NULL;
            }
            arg[i] = NULL;
            arg2[cnt - i - 1] = NULL;

            if ((pid = fork()) == 0) { // child
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                execv(path, arg);
            } else { // parent
                if (fork() == 0) {
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    execv(path2, arg2);
                }
            }
        }
        else {
            if ((pid = fork()) == 0) {
                execv(path, arg);
            }
            wait(NULL);
        }
    }
}