/*************************************************************************
    > File Name: oncerun.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月28日 星期二 17时01分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

//pthread_once_t once = PTHEREAD_ONCE_INIT;
pthread_once_t once = 0;

void run (void)
{
	printf("function run is running in thread %u \n", pthread_self());
}

void * thread1(void * arg)
{
	pthread_t thid = pthread_self();
	printf("current thread 's ID is %u\n", thid);
	pthread_once(&once, run);

	printf("thread1 ends\n");
}

void * thread2(void * arg)
{
	pthread_t thid = pthread_self();
	printf("current thread's id is %u\n", thid);
	pthread_once(&once,run);
	printf("phread2 ends\n");
}

int main()
{
	pthread_t thid1,thid2;

	pthread_create(&thid1,NULL,thread1,NULL);
	pthread_create(&thid2,NULL,thread2,NULL);
	sleep(3);
	printf("main thread exit\n");
	exit(0);

}
