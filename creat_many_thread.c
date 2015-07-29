/*************************************************************************
    > File Name: creat_many_thread.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月29日 星期三 09时15分15秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void Print(int * a)
{
	printf("hello i am %d thread, my id is %u\n", *a, (unsigned int)pthread_self());
	pthread_exit(0);
}

int main()
{
	pthread_t thid;
	int i;
	int status;

	for(i = 0; i < 10; i++)
	{
		printf("i am creating thread %d\n", i);
		pthread_create(&thid, NULL, (void *)Print, &i);
		pthread_join(thid, (void *)&status);
	}
}
