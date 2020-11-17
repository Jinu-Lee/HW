#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h> 

struct msg
{
	long msgtype;
    char text[1000];
	/* implement here */ 
};

struct msg_ack 
{
	long msgtype;
    char text[100];
	/* implement here */ 
};

int main()
{
	/* 
	 * @ int uid: user id 
	 * @ int receiver_id: receveri's id 
	 * */ 
	int uid; 
	int receiver_id; 
	printf("uid : ");
	scanf("%d", &uid);
	getchar(); // flush   
	printf("rid : ");
	scanf("%d", &receiver_id); 
	getchar(); // flush  
		
	/* 
	 * code to get key and QID 
	 *
	 *
	 * */ 
    key_t k1 = ftok(".", uid);
	key_t k2 = ftok(".", receiver_id);
    int qid1 = msgget(k1, IPC_CREAT | 0660);
	int qid2 = msgget(k2, IPC_CREAT | 0660);
	
	if (fork() == 0) 
	{
		struct msg *msg = malloc(sizeof(struct msg));
		struct msg_ack *msg_ack = malloc(sizeof(struct msg_ack));

		while (1)
		{
			/* receive message  */ 
			/* get msg from queue and print out */
            msgrcv(qid1, (void *) msg, sizeof(struct msg), 1, 0);
            printf("RECEIVED %s\n", msg->text);

            time_t t;
            time(&t);
            msg_ack->msgtype = 2;
            sprintf(msg_ack->text, "%d read message at:\t%s", uid, ctime(&t));

			msgsnd(qid2, (void *) msg_ack, sizeof(struct msg_ack), 0);
		}
		free(msg);
        free(msg_ack);		
	}
	else
	{
		struct msg *msg = malloc(sizeof(struct msg));
		struct msg_ack *msg_ack = malloc(sizeof(struct msg_ack));

		while (1) 
		{
			/* send message  */ 
			/* get user input and send to the msg queue*/
            char buf[1000];
            msg->msgtype = 1;

            fgets(buf, 1000, stdin);
			buf[strlen(buf) - 1] = '\0';

            strcpy(msg->text, buf);
            msgsnd(qid2, (void *) msg, sizeof(struct msg), 0);

            msgrcv(qid1, (void *) msg_ack, sizeof(struct msg_ack), 2, 0);
            printf("%s\n", msg_ack->text);

		}
		free(msg);
        free(msg_ack);		
	}	


	return 0;
}
