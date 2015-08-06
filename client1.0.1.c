#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

struct UserInfo
{
	int flag;
	char name[20];
	char passwd[20];	
}user;

int LogIn(int conn_fd)
{
	char stat[20];
	char buf[50];
	int i;
		
	printf("conn_fd : %d\n", conn_fd);
		
	printf("please input flag:");	
	scanf("%d", &user.flag);
	
	printf("please input username: ");
	scanf("%s", user.name);
		
	printf("please input password: ");
	scanf("%s", user.passwd);
	//发送登陆信息
	send(conn_fd, &user, sizeof(user), 0);
	usleep(100);
	
	printf("send ok\n");
//	fflush(stdin);	
	//接受登陆状态
	recv(conn_fd, stat, 20, 0);
	printf("%s\n", stat);
	
	if(strcmp(stat, "LogIn Failed") == 0)
	{
		return  0;	
	}
	if(user.flag == 2)
	{
		return 2;
	}	
	else
	{
		return 1;
	}
	
}


int SendMessage(int conn_fd)
{
	char message[100];
	int ret;
	
	printf("please input message:\n");
	scanf("%s", message);
	
	strcat(message, "&");
	strcat(message, user.name);
	
	printf("mess:%s\n", message);
	
	ret = send(conn_fd, message, sizeof(message), 0);
	printf("****ret : %d***\n", ret);
	usleep(100);
}

int RecvMessage(int conn_fd)
{
	char buf[500];
	int i, j;
	char message[400];
	char from[100];
	char sendto[100];
	
	printf("now is waitting receive\n");
	recv(conn_fd, buf, sizeof(buf), 0);
	printf("received a message: %s\n", buf);

	//解析信息
	memset(sendto, 0, sizeof(sendto));
	for(i = 0; buf[i] != '%' && i < 20; i++)
	{
		sendto[i] = buf[i];
	}
	sendto[i] = 0; i++;
	
	memset(message, 0, sizeof(memset));
	for(j = 0; buf[i] != '&'; i++, j++)
	{			
		message[j] = buf[i];
	}
	message[j] = 0; i++;
	for(j = 0; buf[i] != '\0'; i++, j++)
	{
		from[j] = buf[i];
	}
	usleep(100);	
	printf("%s : %s\n", from, message);	
}

int main(int argc, char **argv) 	
{
	int i;
	int ret;
	int conn_fd;
	int conn_fd1;
	int serv_prot;
	char buf[50];
	pid_t pid;
	struct sockaddr_in serv_addr;
	//INIT	
	serv_prot = 4507;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(serv_prot);		
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(conn_fd < 0)
	{
		perror("socket");
		exit(1);
	}
	//尝试连接
	if((conn_fd1= connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr))) < 0)
	{
		perror("connect");
		exit(1);
	}

	i = LogIn(conn_fd);
	if(i == 0)
	{
		printf("LogIn failed \n");
		exit(0);
	}
	if(i == 2)
	{
		printf("add new user successfully\n");
		printf("please restart client\n");
		exit(0);
	}
		
	//创建接收消息的子进程,父进程发送
	pid = fork();
	if(pid == 0)
	{
		while(1)
			SendMessage(conn_fd);
	}
	if(pid > 0)
	{
		printf("now is pid == 0\n");
		while(1)
			RecvMessage(conn_fd);
	}
}

