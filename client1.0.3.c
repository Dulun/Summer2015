#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>

struct UserInfo
{
	int flag;
	char name[20];
	char passwd[20];	
}user;

struct Package
{
	int order;
	char sendto[20];
	char message[500];
	char from[20];	
};

struct Package a = {0};

int LogIn(int conn_fd)
{
	int stat;
	char buf[50];
	int i;
		
//	printf("conn_fd : %d\n", conn_fd);
		
	printf("please input flag:");	
	scanf("%d", &user.flag);
	fflush(stdin);
	
	printf("please input username: ");
	scanf("%s", user.name);
	fflush(stdin);
		
	printf("please input password: ");
	scanf("%s", user.passwd);
	fflush(stdin);
	//发送登陆信息
	send(conn_fd, &user, sizeof(user), 0);
	usleep(100);
	
	printf("send ok\n");
//	fflush(stdin);	
	//接受登陆状态

	recv(conn_fd, &stat, sizeof(stat), 0);
	printf("%d\n", stat);
	
	if(stat == 0)
	{
		return  0;	
	}
	if(stat == 2)
	{
		return 2;
	}
	
	else
	{
		return 1;
	}	
}

void InputError()
{
	printf("the message you put can not be recongnized , Please reput\n");
}

int SendPackage(int conn_fd)
{
	char buf[600];
	int ret;
	int i, j, k;
	int percent = 0;
	int sharp = 0;
//	struct Package a = {0};
	
	strcpy(a.from, user.name);         //赋值 a.from
	fflush(NULL);
	printf("Please input message : ");
	scanf("%s", buf);	
	//解析
	for(i = 0; i < strlen(buf); i++)
	{
		if(buf[i] == '%')   { percent++;  continue;}		
		if(buf[i] == '#')    { sharp++;     continue;}		
	}
	
	if(percent == 1 && sharp ==0)//标准格式   du%Hello
	{
		a.order = 1;							//赋值a.oder
		 
		memset(a.sendto, 0, sizeof(a.sendto));
		for(i = 0; buf[i] != '%'; i++)				//赋值a.sendto
		{
			a.sendto[i] = buf[i];
		}a.sendto[i++] = 0; memset(a.message, 0, sizeof(a.message));
		
		for(j = 0; buf[i] != '\0'; i++, j++)			//赋值a.message
		{
			a.message[j] = buf[i];
		}a.message[i++] = 0; 
		//for()
		if(send(conn_fd, &a, sizeof(a), 0) == 0)
		{	
			perror("send error");
			return -1;
		}	
		return 1;	
	}
	
	if(percent == 0 && sharp == 1)//命令格式   #show
	{
		if(buf[0] != '#')  {  InputError();  return 0;}
		
		a.order = 2;                                         //赋值order
		
		memset(a.message, 0, sizeof(a.message));		
		for(i = 1, j = 0; buf[i] != 0; i++, j++)                        //赋值message
		{
			a.message[j] = buf[i];
		}
		if(send(conn_fd, &a, sizeof(a), 0) == 0)
		{	
			perror("send error");
			return -1;
		}	
		return 1;		
	}
	
	if(percent == 0 && sharp == 0)           //talking with other 
	{
		a.order =1;
		if(a.sendto[0] == 0)
		{
			InputError();  
			printf("now you are not talking with another user\n");
			return 0;
		}
		
		memset(a.message, 0, sizeof(a.message));            //只需赋值 message
		strcpy(a.message, buf);	
		
		if(send(conn_fd, &a, sizeof(a), 0) == 0)
		{	
			perror("send error");
			return -1;
		}
		return 1;		
	}
	
	else
	{
		InputError();  
		return 0;
	}
	
}

int RecvPackage(int conn_fd)
{
	char buf[500];
	int i, j;
	char message[400];
	char from[100];
	char sendto[100];
	struct Package b;
	
	if(recv(conn_fd, &b, sizeof(b), 0) == 0)
	{
		perror("recv");
		return 0;
	}
	
	if(b.order == 1)
	{
		printf("\n%s: %s\n", b.from, b.message);
		fflush(stdin);
		return 1;
	}
	
	if(b.order == 3)
	{	
		fflush(stdin);	
		printf("\n%s is online\n", b.message);
		fflush(stdin);
		return 1;		
	}
}

void ShowDesk()
{
	printf("flag:  1.登陆  2.注册\n");
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
	ShowDesk();
	
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
		printf("Can not connect with the Server\n");
		perror("connect");
		exit(1);
	}

	i = LogIn(conn_fd);
	printf("debug i = %d\n", i);
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
		{
			//printf("newProcessForClient\n");
			if(SendPackage(conn_fd) == -1)
			{
				break;
			}
		}
	}
	if(pid > 0)
	{
		printf("now is pid == 0\n");
		while(1)
		{
			if(RecvPackage(conn_fd) == 0)
			{
				kill(pid, SIGHUP);
				printf("Systme ERROR Server off line !! Client exit !!\n");
				break;
			}				
		}
	}
}

