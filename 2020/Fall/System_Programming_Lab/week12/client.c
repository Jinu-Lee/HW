#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

void clear_buf(char* buf) {
    memset(buf, 0, MAX_LEN * 2);
}

void send_int(int cfd, int x) {
    char len = x;
    write(cfd, &len, 1);
}

int read_int(int cfd) {
    char x;
    if (read(cfd, &x, 1) == 0) 
        x = 0;
        
    return x;
}

void send_buf(int cfd, char* buf) {
    int len = strlen(buf);
    send_int(cfd, len);

    for (int n = 0; n < len; ) 
        n += write(cfd, buf + n, len - n);
}

int read_buf(int cfd, char* buf) {
    int len = read_int(cfd);

    for (int n = 0; n < len; ) 
        n += read(cfd, buf + n, len - n);

    return len;
}

int main(int argc, char* argv[]) 
{
    int n, fd, cfd;
    struct hostent* h;
    struct sockaddr_in saddr;
    char buf1[MAX_LEN * 2], buf2[MAX_LEN * 2], temp;
    char* host = argv[1];
    int port = atoi(argv[2]);
    pid_t pid;

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    if ((h = gethostbyname(host)) == NULL) {
        printf("invalid hostname %s\n", host);
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

    printf("Insert your name : ");
    scanf("%s", buf1);
    getchar();
    send_int(cfd, 1);
    send_buf(cfd, buf1);

    if ((pid = fork()) == 0) {
        while (1) {
            clear_buf(buf2);
            if (read_int(cfd)) {
                read_buf(cfd, buf2);
                printf("%s", buf2);
            }
        }
    }
    else {
        while (1) {
            fgets(buf1, MAX_LEN, stdin);
            buf1[strlen(buf1) - 1] = '\0';

            if (strcmp(buf1, "quit") == 0) {
                send_int(cfd, 3);
                close(cfd);
                break;
            }

            send_int(cfd, 2);
            send_buf(cfd, buf1);
        }
        kill(pid, SIGINT);
    }

    wait(NULL);
    close(cfd);

    return 0;
}