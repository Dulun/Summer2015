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
#include<fcntl.h>

struct Package
{
	int order;
	char sendto[20];
	char message[500];
	char from[20];	
};

struct OnlineUser
{
	char username[20];
	int conn_fd;
}OnlineUser[12];

int user_num = 0;

struct UserInfo
{
	int flag;
	char name[20];
	char passwd[20];	
};


void ShowUser()
{
	int i ;
	
	for(i = 0; i < user_num; i++)
	{
		printf("user  name: %s\n", OnlineUser[i].username);		
	}
}

void ShowOnlineUser(int conn_fd)
{
	int i ;
	char name[20];
	struct Package a = {0};
	
	a.order = 3;	
	for(i = 0; i < user_num; i++)
	{
		memset(a.message, 0, sizeof(a.message));
		strcpy(a.message, OnlineUser[i].username);
		printf("user  name: %s\n", OnlineUser[i].username);
		
		send(conn_fd,&a, sizeof(a), 0);		
	}
}

void AddUser(char * username, int conn_fd)
{
	strcpy(OnlineUser[user_num].username , username);
	OnlineUser[user_num].conn_fd = conn_fd;
	user_num++;
}

int find_fd(char * username)
 {
 	int i;
 	
 	for(i = 0; i < user_num; i++)
 	{
 		if(strcmp(username, OnlineUser[i].username) == 0)
 		return OnlineUser[i].conn_fd;
 	}
 	if(i == user_num)
 		return -1;
 } 

int  SignUp(int conn_fd, struct UserInfo user) //注册
{
	//struct Userinfo * head, *p, *q;
	int i;
	int fd;
	char username[20];
	char passwd[20];	
	int stat;
	
	printf("this is a new userinfo!\n");
	fd = open("Userinfo", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	close(fd);
	fd = open("Userinfo", O_RDWR | O_APPEND);
	
	write(fd, (void*)&user, sizeof(user));
	close(fd);	
	stat = 2;
	send(conn_fd, &stat, sizeof(stat), 0);
}

int SignIn(int conn_fd, struct UserInfo user)  //登陆
{	
	int i;
	int fd;
	struct UserInfo User;
	
	fd = open("Userinfo", O_RDWR);
	while(1)
	{
		i = read(fd, &User, sizeof(User));
		printf("i = %d\n", i);
		
		if(i == 0)
		{
			return 2;
		}
			
		if((strcmp(user.name, User.name)) == 0)
		{
			if(strcmp(user.passwd, User.passwd) == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}				
		}
	}
}

void SendToAll(struct Package package)
{
	int i;
	int conn_fd;
	
	for(i = 0; i < user_num; i++)
	{
		conn_fd = find_fd(OnlineUser[i].username);
		
		send(conn_fd, &package, sizeof(package),0);
	}
}



void OffLine(char * name)
{
	int i,j;
	char buf[50];
	
	struct Package package = {0};	
	package.order = 1;
	
	strcpy(buf, name);
	strcat(buf, " now is OFFLine!");
	strcpy(package.from, "SERVER");
	strcpy(package.message, buf);
	
	for(i = 0; i < user_num; i++)
	{
		if(strcmp(OnlineUser[i].username, name) == 0)
			break;
	}

	for(; OnlineUser[i +1].conn_fd != 0; i++,j++)
	{
		OnlineUser[i] = OnlineUser[i + 1];
	}	
	
	printf("##server : %s offline \n", OnlineUser[i].username);	
	OnlineUser[i] = OnlineUser[i + 1];
	user_num--;
	
	SendToAll(package);	
}
OnLineHint(char * name)
{
	char buf[30];
	int i;
	struct Package a;
	int conn_fd;
	
	a.order = 1;
	strcpy(buf, name);
	strcat(buf, " now is OnLine!!");
	strcpy(a.from, "SERVER");
	strcpy(a.message, buf);
	
	for(i = 0; i < user_num; i++)
	{
		conn_fd = find_fd(OnlineUser[i].username);
		if(strcmp(OnlineUser[i].username, name) == 0)
		{
			continue;
		}
		send(conn_fd, &a, sizeof(a),0);
	}
}
//void newProcessForClient(int conn_fd, struct sockaddr_in serv_addr, struct OnlineUser * userlist)
void newPthreadForClient(int * conn_fd) 
{
	char buf[500];
	char recvfrom[10];
	char message[500];
	char sendto[10];
	int i, j;
	int stat;
	int CONN_FD;
	struct UserInfo user;
	struct Package package;
	int send_to_fd;
	char oder[20];
	
	CONN_FD = *conn_fd;
	
	printf("##server : now is newProcessForClient \n##Waiting for Log in\n");
	printf("##server : conn_fd : %d\n", CONN_FD);
	if(recv(CONN_FD, &user,sizeof(user), 0) == 0)
	{
		pthread_exit(0);
	}
	
	printf("##server : received %d\n", user.flag);
	printf("##server : received %s\n", user.name);
	printf("##server : received %s\n", user.passwd);
	
	if(user.flag == 1)
	{
		//登陆 signIn
		stat = SignIn(CONN_FD,user);
		printf("stat = %d\n", stat);
		if(stat == 1)
		{
			AddUser(user.name,CONN_FD);
			ShowUser();
			send(CONN_FD, &stat, sizeof(stat), 0);
		}
		else
		{
			send(CONN_FD, &stat, sizeof(stat), 0);
			pthread_exit(0);		
		}
	}
	
	if(user.flag == 2)
	{
		//注册signUp
		SignUp(CONN_FD, user); 
		pthread_exit(0);
	}
//	strcpy(recvfrom ,  user.name);
	OnLineHint(user.name);	
	while(1)  
	{ //解析信息
		printf("##server : waitting for massage form %s\n", user.name);
		memset(buf, 0, sizeof(buf));		
				
		if(recv(CONN_FD, &package, sizeof(package), 0) == 0)
		{
			printf("%s off line\n", user.name);
			OffLine(user.name);
			pthread_exit(0);
		} //下线
		
			
		
		if(package.order == 1 && strcmp(package.sendto, "all") == 0)
			{
				SendToAll(package);
				printf("now is send all\n");
				continue;
			}
		
		send_to_fd = find_fd(package.sendto);
		if(package.order == 1)
		{	
			if(send_to_fd == -1)
			{
				continue;
			}
			if(send(send_to_fd, &package, sizeof(package), 0) == 0)
			{
				printf("send error\n");
				continue;
			}			
		}	
		
		if(package.order == 2)
		{
			if(strcmp(package.message, "show") == 0)
			{
				ShowOnlineUser(CONN_FD);
			}
		}
	}	
}

int main()
{
	int sock_fd , conn_fd;
	int optval;	
	int  ret;
	int name_num;	
	socklen_t cli_len;
	struct sockaddr_in cli_addr, serv_addr;
	pthread_t thid;
	char buf[50];
	char recv_buf[128];	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);	 	
	optval = 1;	
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));
	//初始化服务器地址结构
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4507);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//表示本服务器将处理来自所有网络接口上的连接请求		
	//将套接字绑定到本地端口
	if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror("bind");
		exit(1);
	}
	//监听模式
	if(listen(sock_fd,12) < 0)
	{
		perror("listen");
	}	
	cli_len = sizeof(struct sockaddr_in);
	
	while(1)
	{
		printf("##server : the server is runnnig waitting for connect\n");
		conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
		printf("%s is connected\n", inet_ntoa(cli_addr.sin_addr));	
	
		if(conn_fd < 0)		
		{
			perror("accept");
			exit(1);
		}			
		pthread_create(&thid, NULL, (void*)newPthreadForClient, (void*)&conn_fd);
	}
	
	return 0;
}


