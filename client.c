#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc, char **argv)
{
	int i;
	int ret;
	int conn_fd;
	int conn_fd1;
	int serv_prot;
	char buf[50];
	struct sockaddr_in serv_addr;

	
	//初始化套接字	
	serv_prot = 4507;

	//inet_aton("127.0.0.1", &serv_addr.sin_addr);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.20.112 ");
	serv_addr.sin_port = htons(serv_prot);
	
		
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd < 0)
	{
		perror("socket");
		exit(1);
	}
	//尝试连接
	if(conn_fd1= connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	
	printf("connect successfully\n");
	while(1)
	{
		printf("please input message:");
		gets(buf);
		//write(conn_fd, buf, 50);		
		send(conn_fd, buf,50, 0);
	}
/*	else 
	{
		printf("connect successfully\n");
	}
	//发送数据	
	if(send(conn_fd, "clh",3, 0) < 0)
	{
		perror("send");
	}
	printf("send_data successfully\n");
	close(conn_fd);
	
	return 0;*/
}

