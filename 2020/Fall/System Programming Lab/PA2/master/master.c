#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    char host[16];
    int port, cfd;
    int start, end;
} Slave;

typedef struct {
    Slave* slave;
    int* data;
} arg_struct;

int* data;
int fail, num_slave;
Slave slaves[10];

int new_socket() {
    int cfd;
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(2);
    }
    return cfd;
}

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

void* thread(void* arg) {
    arg_struct* args = (arg_struct*)arg;
    Slave* slave = args->slave;
    int* data = args->data;
    int n, start = slave->start, end = slave->end;

    send_int(slave->cfd, end - start);

    for (int i = start; i < end; i++) {
        send_int(slave->cfd, data[i]);

        data[i] = receive_int(slave->cfd);
    }

    printf("received from IP:%s, PORT:%d\n", slave->host, slave->port);
    for (int i = start; i < end; i++) 
        printf("%d ", data[i]);
    printf("\n");

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
    // Receive data by stdin
    int num_data;
    scanf("%d", &num_data);
    data = malloc(sizeof(int) * num_data);
    for (int i = 0; i < num_data; i++)
        scanf("%d", &data[i]);

    // Check the slave’s info by ‘world.txt’ file 
    FILE* list;
    if ((list = fopen("world.txt", "r")) == NULL) {
        printf("fopen() failed.\n");
        exit(1);
    }

    while (!feof(list)) {
        fscanf(list, "%s %d", slaves[num_slave].host, &slaves[num_slave].port);
        num_slave++;
    }
    fclose(list);

    // Try to connect the slaves
    int cfd = new_socket();
    struct hostent* h;
    struct sockaddr_in saddr;

    for (int i = 0; i < num_slave; i++) {
        if ((h = gethostbyname(slaves[i].host)) == NULL) {
            printf("gethostbyname() failed.\n");
            continue;
        }

        memset((char*)&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        memcpy((char*)&saddr.sin_addr.s_addr, (char*)h->h_addr, h->h_length);
        saddr.sin_port = htons(slaves[i].port);

        if (connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
            slaves[i].cfd = -1;
            fail++;
        }
        else {
            slaves[i].cfd = cfd;
            cfd = new_socket();
        }
    }
    close(cfd);

    // If the master fails to connect all slaves, 
    // print(“all slaves are not available\n”) and exit 
    if (fail == num_slave) {
        printf("all slaves are not available\n");
        free(data);
        exit(3);
    }

    // If the master fails to connect some of the slaves, 
    // forget the slaves that failed to connect and proceeds. 
    Slave temp[10];
    for (int i = 0, j = 0; i < num_slave; i++) 
        if (slaves[i].cfd != -1) 
            memcpy(&temp[j++], &slaves[i], sizeof(Slave));

    num_slave -= fail;
    for (int i = 0; i < num_slave; i++) 
        memcpy(&slaves[i], &temp[i], sizeof(Slave));

    // Split the data (array) into several blocks (subarray) as uniform as possible
    // If the number of data (the length of array) is not divided into the number of connected slaves, 
    // then the slaves receive remainders in the order of world.txt
    for (int i = 0; i < num_data; i++) 
        slaves[i % num_slave].end++;
    
    for (int i = 1; i < num_slave; i++) 
        slaves[i].end += (slaves[i].start += slaves[i - 1].end);

    // For each machine, send a data block using socket 
    arg_struct* args = malloc(sizeof(arg_struct) * num_slave);
    pthread_t* tid = malloc(sizeof(pthread_t) * num_slave);

    for (int i = 0; i < num_slave; i++) {
        args[i].slave = &slaves[i];
        args[i].data = data;

        if (pthread_create(&tid[i], NULL, thread, (void*)&args[i])) {
            printf("ERROR: pthread creation failed.\n");
            exit(4);
        }
    }

    for (int i = 0; i < num_slave; i++)
        pthread_join(tid[i], NULL);

    for (int i = 0; i < num_slave; i++)
        close(slaves[i].cfd);

    free(data);
    free(args);
    free(tid);

    return 0;
}