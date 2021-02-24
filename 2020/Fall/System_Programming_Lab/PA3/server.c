#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_USER 1024
#define MAX_SEAT 256

typedef enum { NOT_CONNECTED = 0, LOGIN, LOGOUT } Status;

typedef struct {
    int pw;
    int connfd;
    int seat;
    Status status;
} User;

typedef struct {
	int user;
	int action;
	int data;
} query;

User user[MAX_USER];
int seat[MAX_SEAT], listenfd;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void int_handler(int sig) {
    printf("close socket\n");
    close(listenfd);
    
    exit(0);
}

void* thread(void* arg) {
    int connfd = *((int*) arg), uid, ret, logged_in_uid = -1;
    query q;

    pthread_detach(pthread_self());
    free(arg);

    while (1) {
        recv(connfd, &q, sizeof(q), 0);
        uid = q.user;

        pthread_mutex_lock(&lock);

        if (!(q.user | q.action | q.data)) {
            send(connfd, seat, sizeof(seat), 0);
            user[uid].connfd = -1;
            user[uid].status = NOT_CONNECTED;
            pthread_mutex_unlock(&lock);
            break;
        }

        switch (q.action) {
        /* 1. Log in */
        case 1:
            /* correct passcode && not logged in at other client && not logged in with other id */
            ret = (((user[uid].pw == -1) || (user[uid].pw == q.data)) && (user[uid].status != LOGIN) && (logged_in_uid == -1)) ? 1 : -1;
            if (ret == 1) {
                logged_in_uid = uid;
                user[uid].pw = q.data;
                user[uid].connfd = connfd;
                user[uid].status = LOGIN;
            }
            break;

        /* 2. Reserve */
        case 2:
            /* logged in && seat number is in 0 ~ 255 && not reserved seat && didn't reserve any seat */
            ret = ((logged_in_uid == uid) && (user[uid].status == LOGIN) && (0 <= q.data && q.data < MAX_SEAT) && (seat[q.data] == -1) && (user[uid].seat == -1)) ? q.data : -1;
            if (ret >= 0) {
                user[uid].seat = q.data;
                seat[q.data] = uid;
            }
            break;

        /* 3. Check reservation */
        case 3:
            /* logged in && reserved */
            ret = ((logged_in_uid == uid) && (user[uid].status == LOGIN) && (user[uid].seat != -1)) ? user[uid].seat : -1;
            break;

        /* 4. Cancel reservation */
        case 4:
            /* logged in && reserved */
            ret = ((logged_in_uid == uid) && (user[uid].status == LOGIN) && (user[uid].seat != -1) && (user[uid].seat == q.data)) ? user[uid].seat : -1;
            if (ret >= 0) {
                user[uid].seat = -1;
                seat[q.data] = -1;
            }
            break;

        /* 5. Log out */
        case 5:
            /* logged in */
            ret = ((logged_in_uid == uid) && (user[uid].status == LOGIN)) ? 1 : -1;
            if (ret == 1) {
                logged_in_uid = -1;
                user[uid].status = LOGOUT;
            }
            break;

        /* action out of range */
        default:
            ret = -1;
        }

        send(connfd, &ret, sizeof(ret), 0);
        pthread_mutex_unlock(&lock);
    }

    close(connfd);
    return NULL;
}

int main(int argc, char* argv[])
{
    struct sockaddr_in saddr, caddr;
    int *connfdp, caddrlen = sizeof(caddr), thread_cnt = 0;
    pthread_t tid;

    signal(SIGINT, int_handler);

    if (argc < 2) {
		printf("argv[1]: port number\n");
		exit(1);
	}

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(2);
    }

    memset((char*)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(atoi(argv[1]));

    if (bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(3);
    }

    if (listen(listenfd, MAX_USER) < 0) {
        printf("listen() failed.\n");
        exit(4);
    }

    /* user array */
    User default_user = {-1, -1, -1, NOT_CONNECTED};
    for (int i = 0; i < MAX_USER; i++)
        user[i] = default_user;

    /* seat array */
    memset(seat, -1, sizeof(seat));

    while (1) {
        connfdp = (int*) malloc(sizeof(int));
        if ((*connfdp = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen)) < 0) {
            printf("accept() failed.\n");
            free(connfdp);
            continue;
        }
        pthread_create(&tid, NULL, thread, connfdp);
    }

    close(listenfd);

    return 0;
}