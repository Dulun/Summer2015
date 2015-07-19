/*************************************************************************
    > File Name: 6.2-my_create.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月18日 星期六 14时42分32秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include<errno.h>

int main()
{
	int fd;

	if((fd = open("example_62.c", O_CREAT | O_EXCL, S_IRUSR| S_IWUSR)) == -1)
	{ // fd = creat("example_62,c", S_IRWXU)
		perror("open");
		//printf("open is %d\n"strerror(errno),errno)
		exit(1);
	}
	else
	{
		printf("creat fiie success \n");
	}

	close(fd);
	return 0;	
}
