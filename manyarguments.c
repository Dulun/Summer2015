/*************************************************************************
    > File Name: pthreadcreat2.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月29日 星期三 08时51分32秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

struct Node
{
	int a;
	int b;

};

int Print(struct Node * a)
{
	printf("a = %d  b =  %d\n", a->a, a->b);
	pthread_exit(0);
}

int main()
{
	pthread_t thid;
	struct Node a;
	a.a = 3;
	a.b = 4;

	pthread_create(&thid, NULL, (void *)Print,&a);
	sleep(1);

}
