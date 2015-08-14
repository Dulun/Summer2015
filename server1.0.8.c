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
#include<string.h>                        					///头文件
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<time.h>								//获取时间函数文件包含

int FD;  //系统日志描述符

struct Package                 //数据包
{
	int order;
	char sendto[20];
	char message[500];
	char from[20];	
};

struct OnlineUser		//在线用户列表
{
	char username[20];
	int conn_fd;
}OnlineUser[12];

int user_num = 0;		//在线用户个数

struct SystmeDiary		//系统日志
{
	char time[50];
	char things[100];
	char name[20];
};



struct UserInfo		//用户信息,(用于登陆)
{
	int flag;			//登陆标志
	char name[20];
	char passwd[20];	
};

void ShowUser()	//	用于调试bug
{
	int i ;
	
	for(i = 0; i < user_num; i++)
	{
		printf("user  name: %s\n", OnlineUser[i].username);		
	}
}
//--------------------------显示在线用户列表, 并转发给请求者---------------------------------------------------
void ShowOnlineUser(int conn_fd) 
{
	int i ;
	char name[20];
	struct Package a = {0};
	
	a.order = 2;	
	for(i = 0; i < user_num; i++)
	{
		memset(a.message, 0, sizeof(a.message));
		strcpy(a.message, OnlineUser[i].username);
		printf("user  name: %s\n", OnlineUser[i].username);
		
		send(conn_fd,&a, sizeof(a), 0);		//转发
	}
}
//---------------------------------  //增加在线用户,--------------------------------------------
void AddUser(char * username, int conn_fd)    
{
	strcpy(OnlineUser[user_num].username , username);
	OnlineUser[user_num].conn_fd = conn_fd;
	user_num++;
	ShowUser();
}

//---------------------------核心技能!!!!----找到要转发的接收者--------------------------------------------------

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
//----------------------------//注册, 写入文件-------------------------------------------------

int  SignUp(int conn_fd, struct UserInfo user) 
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

//------------------------------//登陆, 在文件中查找-----------------------------------------------

int SignIn(int conn_fd, struct UserInfo user)  
{	
	int i;
	int fd;
	struct UserInfo User;
	
	fd = open("Userinfo", O_RDWR);
	while(1)
	{
		if(read(fd, &User, sizeof(User)) < 1)
		{
			return 0;
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

//-------------------------------//群发, 转发给所有在线用户----------------------------------------------

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

//------------------------- //下线, 将下线者从在线用户列表中删除----------------------------------------------------

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
	
	ShowUser();
	SendToAll(package);	
}

//----------------------------上线提醒,,给在线所有用户提醒:某用户已经上线----------------------

void OnLineHint(char * name)      //
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
	
//------------------------------循环发送上线信息, "上线提醒"-------------------------
	
	for(i = 0; i < user_num; i++)						
	{
		conn_fd = find_fd(OnlineUser[i].username);          
		if(strcmp(OnlineUser[i].username, name) == 0)
		{
			continue;
		}
		send(conn_fd, &a, sizeof(a),0);        
	}
//----------------------------------------------------------------------------------------------
}

//------------------------------//发送用户不在线提醒-----------------------------------------------
void SendNotOnlineMessage(int CONN_FD , char * name)   
{
	struct Package a;	
	strcpy(a.message, name);
	strcat(a.message, " is not OnLine!");
	strcpy(a.from, "SERVER");
	a.order = 1;
	
	send(CONN_FD, &a, sizeof(a), 0);
}

//--------------------------------发送离线消息线程函数-------------------------------------------

void	SendOFFLineMessage(struct Package * a)  //发送离线消息线程函数
{
	struct Package package;
	int send_to_fd;
	
	package = *a;	
	while((send_to_fd = find_fd(package.sendto)) < 1)
	{
		sleep(1);
	}
	
	strcat(package.from, "(offline)");
	
	if(send(send_to_fd, &package, sizeof(package), 0) == 0)//转发消息.*******************
	{
		printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$send error\n");
		pthread_exit(1);
	}
	pthread_exit(1);
}
//----------------------------------//服务器核心函数-------------------------------------------

void newPthreadForClient(int * conn_fd)          
{
	char buf[500];                //发送缓冲
	char recvfrom[10];		
	char message[500];
	char sendto[10];
	char diarybuf[200];    		 //聊天记录缓冲区
	char chatdiarybuf[300];		 //聊天记录缓冲区
	int i, j;
	int m, n;
	int stat = 0;				//登陆标志
	int CONN_FD;			//套接字描述符
	struct UserInfo user;		//用户信息结构体
	struct Package package;	//消息结构体
	int send_to_fd;			//sendto 套接字描述符
	char oder[20];		
	time_t timep;				//时间
	struct SystmeDiary diary = {0};  //系统日志结构体
	int chatdiary_fd;			//聊天记录文件描述符
	int chatdiary_FD;			//聊天记录文件描述符
	char name[20];
	int fd;
	pid_t thid;				//发送离线消息线程thid
	
	CONN_FD = *conn_fd;		//转换为局部变量
	
	
	
	printf("##server : now is newProcessForClient \n##Waiting for Log in\n");
//	printf("##server : conn_fd : %d\n", CONN_FD);
	if(recv(CONN_FD, &user,sizeof(user), 0) == 0)   //接收登陆包
	{
		pthread_exit(0);
	}
	
	if(user.flag == 1)
	{	
		time(&timep);		 	
 		strcpy(diarybuf, ctime(&timep));
 		strcat(diarybuf, user.name);
 		strcat(diarybuf, "  want to log in\n\0");
 		write(FD, diarybuf, strlen(diarybuf)); 		
   		
		stat = SignIn(CONN_FD,user);          //验证用户名密码
		printf("stat = %d\n", stat);                   //验证状态
		if(stat == 1)
		{
			AddUser(user.name,CONN_FD);
			ShowUser();
			send(CONN_FD, &stat, sizeof(stat), 0);
		}
		else
		{
			send(CONN_FD, &stat, sizeof(stat), 0);    //发送登陆状态
			pthread_exit(0);		
		}
		
		strcpy(name, user.name);
//		strcat(name, ".txt");
		
		chatdiary_fd = open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);  //写入日志
		close(chatdiary_fd);
		chatdiary_fd = open(name, O_RDWR | O_APPEND);
	}
	
	if(user.flag == 2)
	{
		strcpy(diarybuf, ctime(&timep));
 		strcat(diarybuf, "  ");
 		strcat(diarybuf, user.name);
 		strcat(diarybuf, "want a new count\n\0");

 		write(FD, diarybuf, strlen(diarybuf));
		//注册signUp
		SignUp(CONN_FD, user); 
		pthread_exit(0);
	} 
//-----------------------------------------ROOT---------------------------------	
//-------------------------------------------------------------------------------	
//-------------------------------------------------------------------------------	

	if(user.flag == 3)
	{		
		if((strcmp(user.name, "root") ) == 0)
		{
			if((strcmp(user.passwd, "ROOT")) == 0)        //验证状态
			{			 
				ShowUser();				
				i = 1;
				send(CONN_FD, &i, sizeof(i), 0);
				usleep(50);
			
				while(1)  
				{ 									//解析信息		
						
					if(recv(CONN_FD, &package, sizeof(package), 0) == 0)
					{
						printf("%s off line\n", user.name);
						OffLine(user.name);
						pthread_exit(0);
					} 												//下线		
			
					if(package.order == 1)
					{
						if((strcmp(package.message, "show")) == 0)  //显示聊天记录
						{
							fd = open(package.sendto, O_RDONLY);
							memset(package.message, 0, sizeof(package.message));	
							while(read(fd, package.message, sizeof(package.message)) > 0)
							{
								send(CONN_FD, &package, sizeof(package), 0);
								usleep(100);
								memset(package.message, 0, sizeof(package.message));
							}
							memset(package.message, 0, sizeof(package.message));
							printf("send over\n");
							//close(fd);
							send(CONN_FD, &package, sizeof(package), 0);
							continue;
						}	
						if((strcmp(package.message, "kick")) == 0)         //=-------踢人!!!!! !!!!! !!!!!!!! !!!! !!!!
						{
							if((send_to_fd = find_fd(package.sendto) )== 0)
							{							
								continue;
							}
							
							package.order = 999;	
							printf("send to fd  = %d\n", send_to_fd);							
							send(send_to_fd, &package, sizeof(package), 0);
							close(send_to_fd);							
						//	OffLine(package.sendto);
							printf("hello world!\n");
							continue;		
						}											
					}
					
					if(package.order == 2)                          //显示在线用户列表
					{
						if(strcmp(package.message, "show") == 0)
						{
							ShowOnlineUser(CONN_FD);
						}
					}	
					

				}//while(1)

			}//if( = 0)
			else
			{
				i = 0;
				send(CONN_FD, &i, sizeof(i), 0);   //发送登陆错误
				usleep(50);
			}
	
		}//if == 0
		else
		{										//发送登陆错误
			i = 0;
			send(CONN_FD, &i, sizeof(i), 0);
			usleep(50);
		}
	}//flag == 3
//--------------------------------------------------------------------------------------------------	
//-------------------------------普通用户-------------------------------------------------------------------	
//..............................................................................................................................................................................................................
	OnLineHint(user.name);	
	strcpy(diarybuf, ctime(&timep));
 	strcat(diarybuf, user.name);
 	strcat(diarybuf, "  log in successfully\n\0");
 
 	write(FD, diarybuf, strlen(diarybuf));
	while(1)  
	{ //解析信息
		printf("##server : waitting for massage form %s\n", user.name);
		memset(buf, 0, sizeof(buf));		
				
		if( (m = recv(CONN_FD, &package, sizeof(package), 0)) == 0)
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
		
		send_to_fd = find_fd(package.sendto);	    //核心
		
//---------------------------------服务器核心转发----------------------------------------------//	..............................

		if(package.order == 1 || package.order == 10  ||package.order == 11  || package.order == 111)        //转发
		{			
			if(package.order == 1)
			{
			
//---------------------写入系统日志---------------------------------------------------------//	
	
				strcpy(diarybuf, ctime(&timep));
				strcat(diarybuf, "  ");
 				strcat(diarybuf, package.from);
 				strcat(diarybuf, " send to ");
 				strcat(diarybuf, package.sendto);
 				strcat(diarybuf, " a message ");
 				strcat(diarybuf, "\n\0"); 
 				write(FD, diarybuf, strlen(diarybuf)); 	
 							
 //--------------------写入自己聊天记录------------------------------------------------------------//	
 	
 				memset(chatdiarybuf, 0, sizeof(chatdiarybuf));
 				strcpy(chatdiarybuf, ctime(&timep));
 				strcat(chatdiarybuf, package.from);	
 				strcat(chatdiarybuf, "->");
 				strcat(chatdiarybuf, package.sendto);
 				strcat(chatdiarybuf, " : ");
 				strcat(chatdiarybuf, package.message); 				
 				strcat(chatdiarybuf, "\n\0"); 
 				write(chatdiary_fd, chatdiarybuf, strlen(chatdiarybuf));	 
 								
 //-----------------------写入对方聊天记录--------------------------------------------------------//
 		
 				chatdiary_FD = open(package.sendto, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
				close(chatdiary_FD);
				chatdiary_FD = open(package.sendto, O_RDWR | O_APPEND);				
				memset(chatdiarybuf, 0, sizeof(chatdiarybuf));
 				strcpy(chatdiarybuf, ctime(&timep));
 				strcat(chatdiarybuf, package.from);	
 				strcat(chatdiarybuf, "->");
 				strcat(chatdiarybuf, package.sendto);
 				strcat(chatdiarybuf, " : ");
 				strcat(chatdiarybuf, package.message); 				
 				strcat(chatdiarybuf, "\n\0"); 
 				write(chatdiary_FD, chatdiarybuf, strlen(chatdiarybuf));	
 				close(chatdiary_FD);						
 			}
 			
 			if(package.order == 11)
			{
 //-----------------------写入发送文件请求日志--------------------------------------------------------//			
				strcpy(diarybuf, ctime(&timep));
				strcat(diarybuf, "  ");
 				strcat(diarybuf, package.from);
 				strcat(diarybuf, " send to ");
 				strcat(diarybuf, package.sendto);
 				strcat(diarybuf, " a file ");
 				strcat(diarybuf, "\n\0"); 
 				write(FD, diarybuf, strlen(diarybuf));
 			}
 			
 //			if(package.order == 111)
 //   		{
 //   			F++;
 //   			printf("!!!!!RECV = %d\n", m);
 //   			strcpy(diarybuf, ctime(&timep));
 //   			strcat(diarybuf, "  ");
 //				strcat(diarybuf, package.from);
 //				strcat(diarybuf, " is sending to ");
 //				strcat(diarybuf, package.sendto);
 //				strcat(diarybuf, " a file ");
 //				strcat(diarybuf, "\n\0"); 
 //				write(FD, diarybuf, strlen(diarybuf));
 //				//printf("b.message = %d", strlen(package.message));
 //			}
  //-----------------------如果没找到该用户 提示不在线信息------------------------------------------//			
			if(send_to_fd == -1)
			{
				printf("NOW IS NOT ON LINE , the offline mess has been send\n");
				SendNotOnlineMessage(CONN_FD,package.sendto);
				pthread_create(&thid, NULL, (void*)SendOFFLineMessage, (void*)&package);
				continue;
			}
			
			if((n = send(send_to_fd, &package, sizeof(package), 0)) == 0)//转发消息.*******************
			{
				printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$send error\n");
				continue;
			}	
//			printf("!!!!!SEND = %d\n", n);	
//			printf("COUTN = %d\n", F);	
		}	
//---------------------------------服务器核心----------------------------------------------------		
		if(package.order == 2)
		{
			if(strcmp(package.message, "show") == 0)
			{
				ShowOnlineUser(CONN_FD);
			}
			
			
			if((strcmp(package.message, "history")) == 0)
			{
				fd = open(package.from, O_RDONLY);
				memset(package.message, 0, sizeof(package.message));	
				while(read(fd, package.message, sizeof(package.message)) > 0)
				{
					package.order = 1;
					send(CONN_FD, &package, sizeof(package), 0);
					usleep(100);
					memset(package.message, 0, sizeof(package.message));
				}
				memset(package.message, 0, sizeof(package.message));
				close(fd);
				send(CONN_FD, &package, sizeof(package), 0);
			}	
			
		}//if(package.order == 2)			 	
	}//while(1)	
}//newProcessForClient

int main()
{
	int sock_fd , conn_fd;    //套接字描述符
	int optval;			// optval：指针，指向存放选项值的缓冲区。
//	int  ret;
//	int name_num;		//
	time_t timep;                    //得到时间，存在 timep中
	socklen_t cli_len;            //套接字地址结构长度
	struct sockaddr_in cli_addr, serv_addr;   //
	pthread_t thid;			//线程 id
//	char buf[50];			//
//	char recv_buf[128];	


	sock_fd = socket(AF_INET, SOCK_STREAM, 0);	 	//创建一个TCP套接字
	optval = 1;	
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)); //修改套接字属性,使端口号可再利用
	optval = 50000;
	setsockopt(conn_fd, SOL_SOCKET, SO_SNDBUF, (void *)&optval, sizeof(int));
	setsockopt(conn_fd, SOL_SOCKET, SO_RCVBUF, (void *)&optval, sizeof(int));
	
//----------初始化服务器地址结构--------------------------------------------------------

	bzero(&serv_addr, sizeof(serv_addr));         //设置地址结构,所有字节为零且包括‘\0’
	serv_addr.sin_family = AF_INET;                  //设置地址类型, 对于TCP/IP协议进行的网络编程,该值只能为AF_INET;
	serv_addr.sin_port = htons(4507);			//设置端口号,host to network short 将主机短整型转换成网络字节顺序.
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//表示本服务器将处理来自所有网络接口上的连接请求	
	
		
//-------调用 bind() 函数将套接字和某一个端口绑定 //书上写的.,,----------------------------------

	if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0 )
	{
		perror("bind");
		exit(1);
	}
///----------将套接字转换为被动监听模式-----------------------------------------------

	if(listen(sock_fd,12) < 0)
	{
		perror("listen");
	}	
	cli_len = sizeof(struct sockaddr_in);
	
///------------打开系统日志,	-----------------------------------------------------------------------

	FD = open("diary.txt", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	close(FD);
	FD = open("diary.txt", O_RDWR | O_APPEND);
	
//------------------死循环,为用户创建一个线程------------------------------------------------------------
	while(1)
	{ 
		printf("##server : the server is runnnig waitting for connect\n");                //服务器输出提醒
		conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);  //调用accept阻塞等待链接, 握手成功返回一个新的套接字描述符
		printf("%s is connected\n", inet_ntoa(cli_addr.sin_addr));	
	
		if(conn_fd < 0)		 //连接错误判断 错误返回-1,退出
		{
			perror("accept");
			continue;
		}	
//---------------每连接一个用户开一个线程	线程函数(服务器核心) newProcessForClient(int conn_fd)-----------------
		pthread_create(&thid, NULL, (void*)newPthreadForClient, (void*)&conn_fd);
	}
	
	return 0;
}


