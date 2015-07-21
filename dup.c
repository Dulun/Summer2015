/*************************************************************************
    > File Name: dup.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月20日 星期一 15时27分24秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int fd1;
	int fd2;
	char buf[20];
	char buf2[20];

	fd1 = open("test", O_RDONLY);
//	fd2 = dup(fd1);
	dup2(fd1,fd2);
	
	read(fd1, buf, 20);
	lseek(fd1, 0 ,SEEK_SET);
	read(fd2, buf2, 20);

	printf("fd1 = %d   fd2 = %d\n", fd1, fd2);
	puts(buf);
	puts(buf2);

}
