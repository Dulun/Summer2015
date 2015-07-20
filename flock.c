/*************************************************************************
    > File Name: flock.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月20日 星期一 12时20分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd;
	int ret;
	struct flock lock;
	char read_buf[32];

	fd = open("test", O_CREAT | O_TRUNC |O_RDWR, S_IRWXU);
	write(fd, "hello world", 20);
	memset(&lock, 0, sizeof(struct flock));

	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;
	lock.l_type = F_RDLCK;

	if(fcntl(fd, F_SETLK, &lock) == 0)
	{
		printf("lock succeed for read by %d", getpid());
	}
	
	lseek(fd, 0, SEEK_SET);
	read(fd, read_buf, 20);
	printf("  %s \n", read_buf);

	getchar();// 读锁

	lock.l_type = F_WRLCK;
	if(fcntl(fd,F_SETLK, &lock) == 0)
	{
		printf("lock succeess for WRITE by %d\n", getpid());
	}
	else
	{
		printf("Can not lock \n");
	}

	return 0;

}
