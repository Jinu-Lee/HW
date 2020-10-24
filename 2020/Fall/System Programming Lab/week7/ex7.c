#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int num, count;
pid_t* pid;

void receive(int sig) {
	kill(getppid(), SIGUSR1);
}

void terminate(int sig) {
	printf("receiver: receving signal: %d\n", num);
	exit(0);
}

void send(int sig) {
	printf("sender: sending %d signal\n", num - count);
	for (int i = 0; i < num; i++) {
		kill(pid[i], SIGUSR1);
	}
}

void counting(int sig) {
	pid_t id;
	while ((id = waitpid(-1, NULL, WNOHANG)) > 0) {
		printf("receiver: receive %d signal and sending acks\n", ++count);
	}
}

int main()
{
	printf("sending signal: ");
	scanf("%d", &num);
	pid = (pid_t*) calloc(num, sizeof(pid_t));

	for (int i = 0; i < num; i++) {
		if ((pid[i] = fork()) == 0) {
			signal(SIGUSR1, receive);
			signal(SIGINT, terminate);
			while(1);
		}
	}

	signal(SIGALRM, send);
	signal(SIGUSR1, counting);

	send(0);
	while (1) {
		if (num == count) {
			for (int i = 0; i < num; i++) {
				kill(pid[i], SIGINT);
			}
			printf("all signals are sended\n");
			break;
		}
		else {
			alarm(1);
			pause();
		}
	}

	free(pid);
	return 0;
}

