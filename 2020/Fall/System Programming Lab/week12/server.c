#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define MAX_USER 100

int n_added, n_leaved, cfds[MAX_USER + 3], listenfd;
char name[MAX_USER][MAX_LEN], buf[MAX_LEN * 2];

void clear_buf() {
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

void send_buf(int cfd) {
    int len = strlen(buf);
    send_int(cfd, 1);
    send_int(cfd, len);

    for (int n = 0; n < len; ) 
        n += write(cfd, buf + n, len - n);
}

int read_buf(int cfd) {
    int len = read_int(cfd);

    for (int n = 0; n < len; ) 
        n += read(cfd, buf + n, len - n);

    return len;
}

void add_user(int connfd) {
    int len = read_buf(connfd);
    strncpy(name[n_added], buf, len);

    cfds[n_added++] = connfd;
    sprintf(buf, "%s joined. %d current users.\n", name[n_added - 1], n_added);

    for (int i = 0; i < n_added; i++) {
        if (cfds[i] == connfd || cfds[i] == -1) continue;
        send_buf(cfds[i]);
    }
}

void del_user(int connfd) {
    int idx;
    for (idx = 0; idx < n_added; idx++) {
        if (cfds[idx] == connfd) {
            cfds[idx] = -1;
            break;
        }
    }

    sprintf(buf, "%s leaved. %d current users.\n", name[idx], n_added - (++n_leaved));
    for (int i = 0; i < n_added; i++) {
        if (cfds[i] == -1) continue;
        send_buf(cfds[i]);
    }

    close(connfd);
}

void send_msg(int connfd) {
    char msg[MAX_LEN] = "";
    int len = read_buf(connfd);
    strncpy(msg, buf, len);

    int idx;
    for (idx = 0; idx < n_added; idx++)
        if (cfds[idx] == connfd) break;

    printf("got %d bytes from user %s.\n", len + 1, name[idx]);

    sprintf(buf, "%s:%s\n", name[idx], msg);
    for (int i = 0; i < n_added; i++) {
        if (cfds[i] == connfd || cfds[i] == -1) continue;
        send_buf(cfds[i]);
    }
}

void int_handler() {
    for (int i = 0; i < n_added; i++) {
        if (cfds[i] != -1) {
            close(cfds[i]);
        }
    }
    close(listenfd);
    exit(0);
}

int main(int argc, char* argv[])
{
    struct hostent* h;
    struct sockaddr_in saddr, caddr;
    fd_set readset, copyset;
    int n, fdnum, fdmax, connfd;
    int port = atoi(argv[1]), caddrlen = sizeof(caddr);

    signal(SIGINT, int_handler);

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

    if (listen(listenfd, 10) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    FD_ZERO(&readset);
    FD_SET(listenfd, &readset);
    fdmax = listenfd;

    while (1) {
        memcpy(&copyset, &readset, sizeof(fd_set));
        
        if ((fdnum = select(fdmax + 1, &copyset, NULL, NULL, NULL)) < 0) {
            printf("select() failed.\n");
            exit(4);
        }

        if (fdnum == 0) {
            continue;
        }

        for (int i = 3; i < fdmax + 1; i++) {
            if (FD_ISSET(i, &copyset)) {
                if (i == listenfd) {
                    if ((connfd = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen)) < 0) {
                        printf("accept() failed.\n");
                        continue;
                    }
                    FD_SET(connfd, &readset);
                    if (fdmax < connfd) fdmax = connfd;

                    clear_buf();
                    if ((n = read_int(connfd)) == 1) {
                        add_user(connfd);
                    }
                }
                else {
                    clear_buf();
                    if ((n = read_int(i)) == 2) {
                        send_msg(i);
                    }
                    else if (n == 3) {
                        FD_CLR(i, &readset);
                        del_user(i);
                    }
                }
            }
        }
    }

    close(listenfd);

    return 0;
}