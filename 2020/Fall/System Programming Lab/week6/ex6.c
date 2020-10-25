#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	unsigned int pid;
	time_t t;
	struct tm *tm;	
	int fd;
	char *argv[3];
	char buf[512];
	int fd0, fd1, fd2;

	fd = open("./crontab", O_RDWR);
	pid = fork();
	
	if(pid == -1) return -1;
	if(pid != 0)
		exit(0);
	if(setsid() < 0)
		exit(0);
	if(chdir("/") < 0)
		exit(0);

	umask(0);

	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);

	t = time(NULL);
	tm = localtime(&t);

	while (1)
	{
		buf[0] = '\0';	

		//insert your code
        int idx = read(fd, buf, 512);
        buf[idx] = '\0';
        
        idx = 0;
        char* token;
        char* pos = buf;
        while(token = strtok_r(pos, " ", &pos)) {
            argv[idx] = malloc(sizeof(char) * strlen(token) + 1);
            strcpy(argv[idx++], token);
        }

        int every_min = !(strcmp(argv[0], "*"));
        int every_hour = !(strcmp(argv[1], "*"));

        int min_flag = (every_min || (tm->tm_min == atoi(argv[0])));
        int hour_flag = (every_hour || (tm->tm_hour == atoi(argv[1])));

        if (min_flag && hour_flag) {
			if (fork() == 0) 
		    	execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);
        }

        for(int i = 0; i < 3; i++) {
            free(argv[i]);
        }
        lseek(fd, 0, SEEK_SET);

		t = time(NULL);
		tm = localtime(&t);
	
		sleep(60 - tm->tm_sec % 60);
	}

	return 0;
}