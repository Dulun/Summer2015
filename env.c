/*************************************************************************
    > File Name: env.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月27日 星期一 09时24分25秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

extern char ** environ;

int main(int argc, char ** argv)
{
	int i;

	printf("Argument:\n");

	for(i = 1; i < argc; i++)
	{
		printf("argv[%d] is %s \n", i, argv[i]);

	}
	printf("Environment :\n");

	for(i = 0 ;environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}

	return 0;
}
