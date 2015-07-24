/*************************************************************************
    > File Name: fork3.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月24日 星期五 16时50分59秒
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
	
	pid = fork();
	switch(pid)
	{
		case 0:
			while(1)
			{
				printf("A background process , PID %d \n ,ParentID : %d\n",getpid, getppid());
				sleep(3);
			}
		case -1:
			{
				perror("Process creation falied \n");
				exit(-1); 
			}

		default:
			printf("I am parent process , my pid is %d \n", getpid());
			exit(0);
	}

	return 0;
}
