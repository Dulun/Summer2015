/*************************************************************************
    > File Name: fork2.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月24日 星期五 16时42分05秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	pid_t pid;

	char * msg;
	int k;
	printf("process creation study\n");
	pid = fork();

	switch (pid)
	{
		case 0:
			msg = "child process is running";
			k = 3; break;
		case -1: 
			perror("process creation failed\n");
			break;
		default:
			msg = "parent process is running";
			k = 5;
			break;
	}
	while (k > 0)
	{
		puts(msg);
		sleep(1);
		k--;

	}
	exit(0);


}
