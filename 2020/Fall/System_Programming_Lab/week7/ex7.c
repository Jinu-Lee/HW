#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int num, count_send, count_receive;
pid_t child;

void receive(int sig) {
	printf("receiver: receive %d signal and sending acks\n", ++count_receive);
	kill(getppid(), SIGUSR1);
}

void terminate(int sig) {
	printf("receiver: receving signal: %d\n", count_receive);
	exit(0);
}

void send(int sig) {
	printf("sender: sending %d signal\n", num - count_send);
	for (int i = 0; i < num - count_send; i++) {
		kill(child, SIGUSR1);
	}
}

void count(int sig) {
	count_send++;
}

int main()
{
	printf("sending signal: ");
	scanf("%d", &num);

	if ((child = fork()) == 0) {
		signal(SIGUSR1, receive);
		signal(SIGINT, terminate);
		while(1);
	} 

	signal(SIGALRM, send);
	signal(SIGUSR1, count);

	send(0);
	while (1) {
		if (num == count_send) {
			kill(child, SIGINT);
			printf("all signals are sended\n");
			break;
		}
		else {
			alarm(1);
			pause();
		}
	}

	return 0;
}

