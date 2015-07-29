/*************************************************************************
    > File Name: threadcreat.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月28日 星期二 20时17分46秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void Print()
{
	printf("i am another thread\n");
}

int main()
{
	pthread_t thid;
	int i = 10086;

	pthread_create(&thid, NULL, (void *)Print,NULL);
	pthread_join(thid,&i);
	printf("i = %d\n", i);
	//sleep(1);
}
