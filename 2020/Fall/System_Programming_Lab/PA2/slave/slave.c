#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

extern int foo(int x);
void int_handler(int sig);

int listenfd, port, connfd, caddrlen;

void send_int(int cfd, int x) {
    char buf[4];
    *((int*)buf) = x;
    
    for (int n = 0; n < 4; ) 
        n += write(cfd, buf + n, 4 - n);
}

int receive_int(int cfd) {
    char buf[4];
    for (int n = 0; n < 4; ) 
        n += read(cfd, buf + n, 4 - n);

    return *((int*)buf);
}

int main()
{
    struct hostent* h;
    struct sockaddr_in saddr, caddr;
    
    int n, size, data;
    char num[11];

    signal(SIGINT, int_handler);

    scanf("%d", &port);

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

    while (1) {
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen)) < 0) {
            printf("accept() failed.\n");
            exit(4);
        }

        size = receive_int(connfd);

        while (size--) {
            data = receive_int(connfd);
            data = foo(data);
            send_int(connfd, data);

            sprintf(num, "%d ", data);
            write(1, num, strlen(num) + 1);
        }

        close(connfd);
    }
}

void int_handler(int sig) {
    printf("\nclose socket\n");
    close(listenfd);
    close(connfd);

    exit(0);
}