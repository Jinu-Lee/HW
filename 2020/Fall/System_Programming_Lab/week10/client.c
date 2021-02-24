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
    int n, fd, cfd;
    struct hostent* h;
    struct sockaddr_in saddr;
    char buf[MAX_LEN + 6], temp;
    char* host = argv[1];
    int port = atoi(argv[2]);

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

    while (1) {
        scanf("%s", buf);
        if (strcmp(buf, "quit") == 0) {
            temp = '\0';
            write(cfd, &temp, 1);
            break;
        }

        n = strlen(buf);
        temp = (n / 10) + '0';
        write(cfd, &temp, 1);
        temp = (n % 10) + '0';
        write(cfd, &temp, 1);

        if (write(cfd, buf, n) == -1) {
            printf("write() failed.\n");
            exit(4);
        }

        if ((fd = open(buf, O_RDONLY)) < 0) {
            printf("open() failed.\n");
            exit(5);
        }

        while (read(fd, &temp, 1) > 0) {
            write(cfd, &temp, 1);
        }
        temp = '\0';
        write(cfd, &temp, 1);

        close(fd);
    }
    close(cfd);

    return 0;
}