/*************************************************************************
    > File Name: pthread_mutex_lock.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月30日 星期四 08时42分44秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
void write_a();
int read_a();

int a;

pthread_mutex_t number_mutex;

int main()
{
	pthread_t thid1, thid2;

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_create(&thid1, NULL, (void *)read_a, NULL);
	printf("main:a = %d\n", a);
	pthread_create(&thid2, NULL, (void *)write_a, NULL);
	printf("main:a = %d\n", a);
	pthread_create(&thid1, NULL, (void *)read_a, NULL);
	printf("main:a = %d\n", a);


}


void write_a()
{
	pthread_mutex_lock(&number_mutex);
	a++;
	pthread_mutex_unlock(&number_mutex);
}

int read_a()
{
	int temp;
	pthread_mutex_lock(&number_mutex);
	temp = a;
	pthread_mutex_unlock(&number_mutex);
	printf("a = %d\n", a);
}
