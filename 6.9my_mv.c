/*************************************************************************
    > File Name: 6.9my_mv.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月19日 星期日 17时12分33秒
 ************************************************************************/

#include<stdio.h>

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("my_mv<old file> <new file>");
		exit(0);
	}

	if(rename(argv[1], argv[2]) < 0)
	{
		perror("rename");
		exit(1);
	}

	return 0;
}
