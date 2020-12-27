#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int connfd, listenfd;
char mode;

void send_info(int mode) {
    char path[100];
    char* command = (mode) ? "ls" : "pwd";
    
    sprintf(path, "/bin/%s", command);
    if (fork() == 0) {
        dup2(connfd, STDOUT_FILENO);
        execl(path, command, NULL);
    }
    wait(NULL);
}

void int_handler(int signo) {
    close(connfd);
    close(listenfd);
    exit(0);
}

void alarm_handler(int signo) {
    send_info(0);
    if (mode) send_info(1);
    alarm(1);
}

int main(int argc, char* argv[])
{
    struct hostent* h;
    struct sockaddr_in saddr, caddr;
    int port = atoi(argv[1]), caddrlen = sizeof(caddr);

    signal(SIGINT, int_handler);
    signal(SIGALRM, alarm_handler);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char*)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 1) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    if ((connfd = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen)) < 0) {
        printf("accept() failed.\n");
    }

    read(connfd, &mode, sizeof(mode));

    alarm(1);
    while (1) {
        pause();
    }

    return 0;
}