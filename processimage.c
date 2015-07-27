/*************************************************************************
    > File Name: processimage.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 09时39分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char ** argv, char ** environ)
{
	int i;

	printf("i ma process image\n");
	printf("my pid = %d , parentpid = %d\n", getpid(), getppid());
	printf("uid = %d, gid = %d\n", getuid(), getgid());

	for(i = 0; i < argc ; i++)
	{
		printf("argv[%d] is %s\n", i, argv[i]);

	}
}
