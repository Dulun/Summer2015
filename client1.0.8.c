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
#include<time.h>
#include <termios.h> 
#include<fcntl.h>

int BOTTOM = 0;
int CONN_FD;
char FILENAME[20];
int count = 0;

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

//----------------------------------getch----------------------------------------------
int getch(void) {  
    struct termios tm, tm_old;  
    int fd = STDIN_FILENO, c;  
    
    if(tcgetattr(fd, &tm) < 0)  
  	  return -1;  
    tm_old = tm;  
    cfmakeraw(&tm);  
    
    if(tcsetattr(fd, TCSANOW, &tm) < 0)  
  	  return -1;  
    c = fgetc(stdin);  
    
    if(tcsetattr(fd, TCSANOW, &tm_old) < 0)  
  	  return -1;  
    return c;  
} 
//-------------------------------------     登陆/注册    -------------------------------------------
int LogIn(int conn_fd)
{
	int stat;
	char buf[50];
	int i;
	char Initpass[20];
	int keycount;		
//	printf("conn_fd : %d\n", conn_fd);		
	printf("please input flag:");	
//	scanf("%c", &user.flag);
	i = getchar();

	
	if(i == 49)
	{
		user.flag = 1;						//正常用户登陆
	}
	if(i == 50)
	{
		user.flag = 2;						//用户申请帐号
	}
	if(i == 51)
	{
		user.flag = 3;						//***************************
	}
	else if(i != 49 && i != 50 && i != 51)
	{
		printf("InputError , Please restart Client!\n");     //错误判断
		exit(0);
	}
	printf("please input username: ");
	scanf("%s", user.name);                            //输入输出
	fflush(stdin);
		
	printf("please input password: ");
	fflush(stdin);	
	getch(); //清除缓存

//------------------------------------------输入密码回显" * " 号------------------------------	
	for(keycount = 0; keycount < 20; keycount++)
	{
		Initpass[keycount] = getch();
		if(Initpass[keycount] == 127)
		{	
			if(keycount == 0)
			{
				keycount --;
				continue;
			}
			printf("\b");
			printf(" ");
			printf("\b");
			keycount -= 2;					
			continue;
		}
			
		if(Initpass[keycount] == 13)    
		{                        //回车结束
			Initpass[keycount] = 0;
			break;
		}
		printf("*");
	}
	strcpy(user.passwd, Initpass);			
	//发送登陆信息
	send(conn_fd, &user, sizeof(user), 0);
	usleep(100);
	
	printf("\nSend to check Successfully\n");       //提示发送成功, 等待服务器响应
//	fflush(stdin);	
//-----------------------------------接受登陆状态-----------------------------

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
//------------------------------------返回登陆状态---------------------------------
void InputError()
{
	printf("the message you put can not be recongnized , Please reput\n");
}
 
//---------------------------------解析输入并发送-------------------------------------------
int SendPackage(int conn_fd)
{
	char buf[600];	
	int ret;
	int i, j, k;
	int percent = 0;
	int sharp = 0;
	int fd;	
//	struct Package a = {0};
	
	strcpy(a.from, user.name);         //赋值 a.from
	fflush(NULL);
	usleep(500);
	printf("Please input message(send to \033[37m%s\033[0m): ", a.sendto);
	//printf("\033[37m\033[1m%s:%s\033[0m\n",  b.from, b.message);  //发送者变色
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
		if(strcmp(buf, "y") == 0)
		{
			BOTTOM = 1;
			printf("Recving the File\n");
		 	a.order = 11;
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
////---------------------------请求发送文件----------------------------------
	if(sharp == 2 && a.order ==1)
	{
		if(buf[0] != '#' || buf[1] != '#')
		{
			InputError();  
			return 0;
		}		
		  //请求标志
//		printf("buf:%s\n", buf);
		memset(a.message, 0, sizeof(a.message));            //只需赋值 message
		for(i = 0, j = 0 ; buf[i] != 0 ; i++)
		{
			if(buf[i] == '#')
				continue;
			FILENAME[j++] = buf[i];
		}
		
		fd = open(FILENAME, O_RDONLY);
		if(fd == -1)
		{
			printf("There is no such file in now dir \n");
			return 1;
		}
		close(fd);	
		a.order = 10;
		
		fd = open("temp.txt", O_CREAT , S_IRUSR | S_IWUSR);
		close(fd);
		fd = open("temp.txt", O_RDWR | O_TRUNC);
		write(fd, FILENAME, sizeof(FILENAME));
		
//		printf("debug#### filename %s\n", FILENAME);
		strcpy(a.message, buf);
		
		send(conn_fd, &a, sizeof(a), 0);
		printf("Waiting for confirm !\n");			
	}
		
	else
	{
		printf("sharp: %d, percent: %d", sharp, percent);
		InputError();  
		return 0;
	}	
}
//--------------------------------------文件发送线程------------------------------------------

int TranceFile(struct Package * b)
{
	struct Package a = *b;
	int fd;
	char buf[500];
	int conn_fd;
	char filename[20];
	char temp[20];
	int ret;
	int i;
	
	strcpy(temp, a.sendto);
	strcpy(a.sendto, a.from);
	strcpy(a.from, temp);	
	fd = open("temp.txt", O_RDONLY);	
	read(fd, filename, sizeof(filename));
	close(fd);	
	conn_fd = a.order;
	a.order = 111;
	remove("temp.txt");	
	
	fd = open(filename, O_RDONLY);	
	memset(a.message, 0, sizeof(a.message));	
		
	while((ret = read(fd, a.message, sizeof(a.message)) )> 0)
	{
//		printf("!!!!!!!!!READ     = %d\n", ret);
		i = send(conn_fd, &a, sizeof(a), 0);
		
//		printf("!!!!!!!!!SEND     = %d\n", i);
//		usleep(500);
		memset(a.message, 0, sizeof(a.message));
//		printf("!!!!!!!!!!!count = %d\n", count++);
	}
	memset(a.message, 0, sizeof(a.message));
	strcpy(a.message, "end");
	send(conn_fd, &a, sizeof(a), 0);
	printf("Send  the File over\n");
	close(fd);
}

//------------------------------------接收------------------------------------------------

int RecvPackage(int conn_fd)
{
	char buf[500];
	int i, j;
	char message[400];
	char from[100];
	char sendto[100];
	char choose;
	struct Package b; 
	time_t timep;
	pthread_t thid;
	int BOTTOM = 0;	
	int count = 0;
	
	if(recv(conn_fd, &b, sizeof(b), 0) == 0)
	{
		perror("recv");
		return 0;
	}		
//	printf("debug recv :b.order = %d\n", b.order);
	
	if(b.order == 1)                                 //解析
	{	
		time(&timep);		
   		printf("%sreceived a message ", ctime(&timep));
   		printf("\033[37m\033[1m%s:%s\033[0m\n",  b.from, b.message);  //发送者变色
		fflush(stdin);
		return 1;
	}
	
	if(b.order == 2)                   //在线用户显示
	{	
		fflush(stdin);	
		printf("\n%s is online\n", b.message);
		fflush(stdin);
		return 1;		
	}

	
	if(b.order == 10)   //接收文件请求
	{
		BOTTOM = 1;
		printf("%s want to trance file :%s to you\n", b.from, b.message);			
		memset(FILENAME, 0, sizeof(FILENAME));
		strcpy(FILENAME, b.message);	
		
		
	//	printf("debug filename %s", FILENAME);		
		printf("choose  y/n\n");			
	}
	
	if(b.order == 11)
	{
		BOTTOM = 0;		
//		printf("debug filename %s\n", FILENAME);
		if(strcmp(b.message, "y") == 0 || strcmp(b.message, "Y") == 0)
		{
			b.order = conn_fd;
			printf("\nThe other side accepted  now is Sending the FILE!\n");
			pthread_create(&thid, NULL, (void*)TranceFile, (void*)&b);
		}
	}
	
	if(b.order == 111)
	{
		int fd;

		if(strcmp(b.message, "end") == 0)
		{
			printf("\nRECV OVER!\n");
			return 1;
		}
		fd = open(FILENAME, O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
		close(fd);
		fd = open(FILENAME, O_RDWR | O_APPEND);		
		write(fd, (void*)&b.message, strlen(b.message));
		close(fd);	
		count ++;
		//printf("!!!!!!!!!!!count = %d\n", count);
	}
	if(b.order == 999)
	{
		return 0;
	}
	
	return 1;
}

void ShowDesk()
{
	printf("flag:  1.登陆  2.注册\n");
}

int main(int argc, char **argv) 	
{
	int i;                              //登陆标志
	int ret;
	int conn_fd;		//套接字描述符
	int conn_fd1;		//套接字描述符状态
	int serv_prot;		//端口
	int optval;		//setsockopt参数
	char buf[50];
	pid_t pid;
	struct sockaddr_in serv_addr;	

	
	system("clear");
	ShowDesk();                                   //显示界面
	
	optval = 50000;	
//-------------------初始化套接字-------------------------------------	
	serv_prot = 4507;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = inet_addr("192.168.20.106"); 
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(serv_prot);		
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	
//-----------------------------------------------------------------
	
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
//-----------------------------设置发送接收缓冲区----------------------------------------------
	setsockopt(conn_fd, SOL_SOCKET, SO_SNDBUF, (void *)&optval, sizeof(int));
	setsockopt(conn_fd, SOL_SOCKET, SO_RCVBUF, (void *)&optval, sizeof(int));
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
		
	printf("\033[33m\033[1mLogIn Successfully ! Welcome to Chat RooM  ___made by Dr.Lun\033[0m\n");  //发送者变色
		
	pid = fork();           //创建进程 
	
	if(pid == 0)           //父进程接收消息, 接收前阻塞
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
	if(pid > 0)       //子进程负责发送消息  ,,发送前阻塞
	{
		printf("now is pid == 0\n");
		while(1)
		{
			if(RecvPackage(conn_fd) == 0)          //返回0 说明接受到0,,服务器断开
			{
				kill(pid, SIGHUP);                       // 干掉子进程,退出客户端
				printf("Systme ERROR Server off line !! Client exit !!\n");
				break;
			}				
		}
	}	
	return 0;
}

