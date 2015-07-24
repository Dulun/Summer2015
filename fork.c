/*************************************************************************
    > File Name: fork.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月24日 星期五 16时14分43秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	pid_t pid;

	printf("Process Creation Study\n");
	pid = fork();
	if(pid == 0) {
		printf("child process is runing, CurPid is %d ParentPid is %d\n", pid, getpid());
	}

	if(pid == -1){
		perror("Process creation failed");
	}

	if(pid != 0 && pid != -1){
		printf("Parent process is running, Child is %d ,ParentPid is %d\n", pid, getpid());
	}

	
}
