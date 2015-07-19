/*************************************************************************
    > File Name: 6.4fcntl_access.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月18日 星期六 16时12分14秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void my_err(const char * err_string, int line)
{
	fprintf(stderr, "line;%d ", line);
	perror(err_string);
	exit(1);
}

int main()
{
	int ret;
	int access_mode;
	int fd;

	if((fd = open("example_64", O_CREAT | O_RDWR, S_IRWXU)) == -1)
	{
		my_err("open", __LINE__);
	}

	if((ret = fcntl(fd, F_SETFL, O_APPEND)) < 0)
	{
		my_err("fcntl", __LINE__);
	}

	access_mode = ret & O_ACCMODE;
	if(access_mode == O_RDONLY)
	{
		printf("example_64 access mode:read only\n");
	}
	else if(access_mode == O_WRONLY)
	{
		printf("example_64 access mode : write only\n");
	}

	else if(access_mode == O_RDWR)
	{
		printf("example_64 access mode: read & write");
	}
	
	if(ret & O_APPEND)
	{
		printf(", append");
	}

	if(ret & O_NONBLOCK)
	{
		printf(", nonblock");
	}
	if(ret & O_SYNC)
	{
		printf(", sync");
	}

	return 0;
}
