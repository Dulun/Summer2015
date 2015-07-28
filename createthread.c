/*************************************************************************
    > File Name: createthread.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月28日 星期二 16时23分10秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

int * thread(void * arg)
{
	pthread_t newthid;

	newthid = pthread_self();
	printf("this is a nwe thread, thread ID is d:%d u: %u \n ", newthid, newthid);

	return NULL;

}

int main()
{
	int thid;
	printf("main thread,ID is %u \n", pthread_self());

	if(pthread_create(&thid, NULL, (void *)thread, NULL) != 0)
	{
		printf("thread creation failed\n");
		exit(0);

		
	}
	sleep(1);
	exit(0);
}
