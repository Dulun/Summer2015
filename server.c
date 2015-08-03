/*************************************************************************
    > File Name: my_server.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年08月03日 星期一 10时48分11秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/socket.h>

int main()
{
	int sock_fd , conn_fd;
	int optval;
	
	int  ret;
	int name_num;
	pid_t pid;
	socklen_t cli_len;
	struct sockaddr_in cli_addr, serv_addr;
	char buf[50];
	char recv_buf[128];
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	//初始化服务器地址结构
	//memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4507);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	//将套接字绑定到本地端口
	if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror("bind");
		exit(1);
	}

	if(listen(sock_fd,12) < 0)
	{
		perror("listen");
	}	
	cli_len = sizeof(struct sockaddr_in);
	
	conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);			
	if(conn_fd < 0)		
	{
		perror("accept");
		exit(1);
	}
	
	printf("accept a new client, ip is %s\n", inet_ntoa(cli_addr.sin_addr));
	
	while(1)
	{
		printf("\n");
		recv(conn_fd, &buf, 50, 0);
		printf("get message :%s\t\tfrom %s", buf,inet_ntoa(cli_addr.sin_addr));	
		memset(&buf, 0, 50);
		fflush(stdin);
	}

	return 0;
}


