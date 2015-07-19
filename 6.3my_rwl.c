/*************************************************************************
    > File Name: 6.3my_rwl.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月18日 星期六 15时12分36秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include <string.h>

void my_err(const char * err_string, int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}

int my_read(int fd)
{
	int len;
	int ret;
	int i;
	char read_buf[64];
	//获取文件长度并保持文件读写指针在文件开始处
	if(lseek(fd, 0, SEEK_END) == -1)
	{
		my_err("lseek", __LINE__);
	}

	if((len = lseek(fd, 0, SEEK_CUR)) == -1)
	{
		my_err("lseek", __LINE__);
	}

	printf("len: %d\n", len);

	//读数据
	if((ret = read(fd, read_buf, len)) < 0)
	{
		my_err("read", __LINE__);
	}
	//打印
	for(i = 0; i < len ; i++)
	{
		printf("%c", read_buf[i]);
	}

	printf("\n");

	return ret;
}

int main()
{
	int fd;
	char write_buf[32] = "Hello World!";

	//创建文件
	if((fd = open("example63.c", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1)
	{
		my_err("open", __LINE__);
	}

	else
	{
		printf("cheat ifle success\n");
	}

	//写数据
	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf))
	{
		my_err("write", __LINE__);
	}
	my_read(fd);

	printf("/*-------------------------------*/\n ");
	if(lseek(fd, 10, SEEK_END) == -1)
	{
		my_err("write", __LINE__);
	}

	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf))
	{
		my_err("write", __LINE__);
	}

	my_read(fd);

	close(fd);

	return 0;
}
