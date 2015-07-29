/*************************************************************************
    > File Name: tsd.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月29日 星期三 10时06分49秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

pthread_key_t key;
//int tsd = 5;
void * thread2(void * arg)
{
	int tsd = 5;
	printf("thread %d is running\n", (int)pthread_self());
 //	pthread_setspecific(key, (void *)tsd);
	printf("thread %d returns %d\n", (int)pthread_self(),(int) pthread_getspecific(key));
}

void * thread1(void *arg)
{
	int tsd = 0;
	pthread_t thid2;
	printf("thread %d is running\n",(int) pthread_self());
	pthread_setspecific(key, (void *)tsd);
	tsd = 0;
	printf("thread %d returns %d\n",(int)pthread_self(), (int)pthread_getspecific(key));
	sleep(1);
	pthread_create(&thid2, NULL, thread2, NULL);
}

int main(void)
{
	pthread_t thid1;
	printf("main thread begins running \n");
	pthread_key_create(&key, NULL);
	pthread_create(&thid1, NULL, thread1, NULL);
	sleep(3);
	pthread_key_delete(key);
	printf("man thread exit \n");

	return 0;
}
