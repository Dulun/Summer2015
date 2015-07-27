/*************************************************************************
    > File Name: getpid.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 15时29分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main()
{
	pid_t pid;

	if((pid = fork()) == -1)
	{
		printf("fork error\n");
		exit(1);
	}

	if(pid == 0)
	{
		printf("getpid return %d \n", getpid());
	}
	exit(0);
    
}
