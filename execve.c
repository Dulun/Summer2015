/*************************************************************************
    > File Name: execvc.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 09时44分43秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char * argv[],char **environ)
{
	pid_t pid;
	int stat_val;
	printf("exec example!\n");
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("process creat failed");
		case 0:
			printf("child process is running\n");
			printf("my pid = %d, parentpid = %d\n", getpid(), getppid());
			printf("uid = %d, gid = %d\n", getuid(), getgid());
			execve("processimage", argv, environ);
			//代码段改变
			printf("process never go to heer\n");
			exit(0);
		default:
			printf("parent process is running\n");
			break;
	}
	wait(&stat_val);
	exit(0);

}
