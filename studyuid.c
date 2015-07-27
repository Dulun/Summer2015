/*************************************************************************
    > File Name: studyuid.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 15时41分36秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

extern int errno;

int main()
{
	int fd;

	printf("uid study\n");
	printf("process's uid = %d, euid = %d\n", getuid(), geteuid());

	if((fd = open("test.c", O_RDWR)) == -1)
	{
		printf("openfailure m errno is %d : %s\n", errno, strerror(errno));
		exit(1);
	}
	else
	{
		printf("Open successfully\n");
	}
	close(fd);

	exit(0);
}

