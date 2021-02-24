#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_LEN 10000

int cfd;

void int_handler(int signo) {
    close(cfd);
    exit(0);
}

int main(int argc, char* argv[])
{
    struct hostent *h;
    struct sockaddr_in saddr;
    char buf[MAX_LEN], mode = atoi(argv[3]), temp;
    char* host = argv[1];
    int n, cfd, port = atoi(argv[2]);

    signal(SIGINT, int_handler);

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("socket() failed.\n");
        exit(1);
    } 

    if ((h = gethostbyname(host)) == NULL) {
        printf("gethostbyname() failed.\n");
        exit(2);
    }

    memset((char*)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char*)&saddr.sin_addr.s_addr, (char*)h->h_addr, h->h_length);
    saddr.sin_port = htons(port);

    if (connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        printf("connect() failed.\n");
        exit(3);
    }

    write(cfd, &mode, sizeof(mode));
    
    while(1) {
        if ((n = read(cfd, buf, MAX_LEN)) == 0) 
            continue;
        buf[n] = '\0';
        write(STDOUT_FILENO, buf, n + 1);
    }
}