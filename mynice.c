/*************************************************************************
    > File Name: mynice.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 16时11分17秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/resource.h>

int main()
{
	pid_t pid;
	int stat_val = 0;
	int oldpir, newpri;

	printf("nice study\n");

	pid = fork();
	switch(pid)
	{
		case 0:
			printf("Child is running ,Curpid is %d, ParentPid is %d\n", getpid(), getppid());
			oldpir = getpriority(PRIO_PROCESS, 0);
			printf("old priority = %d\n", oldpir);

			newpri = nice(2);
			printf("New priority = %d\n", newpri);
			exit(0);
			break;

		case -1:
			perror("Process creation failed\n");
			break;
		
		default:
			printf("Parent is running ChildPid is %d , ParentPId is %d\n", pid, getpid());
	}

	wait(&stat_val);
	exit(0);
}
