#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50

int main(int argc, char* argv[])
{
    int n, listenfd, fd, connfd, caddrlen;
    struct hostent* h;
    struct sockaddr_in saddr, caddr;
    char buf[MAX_LEN + 6], temp;
    int port = atoi(argv[1]);

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

    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }    

    while (1) {
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen)) < 0) {
            printf("accept() failed.\n");
            continue;
        }

        while (1) {
            read(connfd, &temp, 1);
            if (!temp) break;
            n = (temp - '0') * 10; 
            read(connfd, &temp, 1);
            n += (temp - '0');

            n = read(connfd, buf, n);
            buf[n] = '\0';
            printf("File name : %s\n", buf);

            if (strcmp(buf, "quit") == 0) break;

            strcpy(buf + n, "_copy");
            if ((fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR)) < 0) {
                printf("open() failed.\n");
                break;
            }

            n = 0;
            while (read(connfd, &temp, 1) > 0) {
                if (!temp) break;
                write(fd, &temp, 1);
                n++;
            }
            printf("got %d bytes from client.\n", n);

            close(fd);
        }
        
        printf("connection terminated.\n");
        close(connfd);
    }
}