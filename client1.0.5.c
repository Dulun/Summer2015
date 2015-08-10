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
		user.flag = 1;
	}
	if(i == 50)
	{
		user.flag = 2;
	}
	else if(i != 49 && i != 50)
	{
		printf("InputError , Please restart Client!\n");
		exit(0);
	}
	printf("please input username: ");
	scanf("%s", user.name);
	fflush(stdin);
		
	printf("please input password: ");
//	scanf("%s", user.passwd);
	fflush(stdin);
	
	getch();
	
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
	
	printf("\nSend to check Successfully\n");
//	fflush(stdin);	
	//接受登陆状态

	recv(conn_fd, &stat, sizeof(stat), 0);
//	printf("%d\n", stat);
	
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
			printf("bottom %d\n", BOTTOM);
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
		a.order = 10;  //请求标志
		printf("buf:%s\n", buf);
		memset(a.message, 0, sizeof(a.message));            //只需赋值 message
		for(i = 0, j = 0 ; buf[i] != 0 ; i++)
		{
			if(buf[i] == '#')
				continue;
			FILENAME[j++] = buf[i];
		}
		fd = open("temp.txt", O_CREAT , S_IRUSR | S_IWUSR);
		close(fd);
		fd = open("temp.txt", O_RDWR | O_TRUNC);
		write(fd, FILENAME, sizeof(FILENAME));
		
		printf("debug#### filename %s\n", FILENAME);
		strcpy(a.message, buf);
		
		send(conn_fd, &a, sizeof(a), 0);			
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
	int ret;
	int i, j;
	char filename[20];
	char temp[20];
	
	strcpy(temp, a.sendto);
	strcpy(a.sendto, a.from);
	strcpy(a.from, temp);
	
	printf("this is trance pthread\n");
	
	fd = open("temp.txt", O_RDONLY);
	
	read(fd, filename, sizeof(filename));
	close(fd);
	
	conn_fd = a.order;
	printf("conn_fd = %d\n", conn_fd);
	a.order = 111;
	printf("a.order = %d\n", a.order);	
	remove("temp.txt");
	
	printf("123 sendto:%s", a.sendto);
	


	printf("filename@@@@@@: %s\n", filename);
	
	
	fd = open(filename, O_RDONLY);	
	printf("open  fd  =  %d\n", fd);
	memset(a.message, 0, sizeof(a.message));	
	
	while(read(fd, a.message, sizeof(a.message)) > 0)
	{
		printf("i am sending !!\n");
		printf("a.message:%s\n", a.message);
		i = send(conn_fd, &a, sizeof(a), 0);
		printf("hahahah  i = %d", i);
		usleep(100);
		memset(a.message, 0, sizeof(a.message));
	}
	printf("i am end of file !!\n");
	
	memset(a.message, 0, sizeof(a.message));
//Z	strcpy(a.message, "end");
//	send(conn_fd, &a, sizeof(a), 0);
	printf("send over\n");
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
	
	
	if(recv(conn_fd, &b, sizeof(b), 0) == 0)
	{
		perror("recv");
		return 0;
	}	
	
	printf("debug recv :b.order = %d\n", b.order);
	
	if(b.order == 1)                                 //解析
	{	
		time(&timep);		
   		printf("%sreceived a message ", ctime(&timep));
   	//	fflush(stdin);  
   		printf("\033[37m\033[1m%s:%s\033[0m\n",  b.from, b.message);  //发送者变色
	//	printf("%s: %s\n", b.from, b.message);
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
	
///	if(b.order == 3)
//	{		
//		pthread_create();  //创建发送文件线程		
//	}
	
	if(b.order == 10)   //接收文件请求
	{
		BOTTOM = 1;
		printf("%s want to trance file :%s to you\n", b.from, b.message);	
		
		memset(FILENAME, 0, sizeof(FILENAME));
		strcpy(FILENAME, b.message);	
	
		printf("debug filename %s", FILENAME);		
		printf("choose  y/n\n");				
	}
	
//	if(b.order ==11 && BOTTOM == 1)     //是否接受文件
	if(b.order == 11)
	{
		BOTTOM = 0;
		
//		if(b.message[0] == 'y' || b.message[0] == 'Y')
		printf("debug filename %s\n", FILENAME);
		if(strcmp(b.message, "y") == 0 || strcmp(b.message, "Y") == 0)
		{
			printf("b.order ==11 && BOTTOM == 1\n");
			b.order = conn_fd;
			pthread_create(&thid, NULL, (void*)TranceFile, (void*)&b);
		}
	}
	
	if(b.order == 111)
	{
		int fd;
		
		printf("REVICED is a new FILE!\n");

		fd = open(FILENAME, O_CREAT | O_EXCL | O_TRUNC	, S_IRUSR | S_IWUSR);
		close(fd);
		fd = open(FILENAME, O_RDWR | O_APPEND);
		
		write(fd, (void*)&b.message, strlen(b.message));
		close(fd);	
	}
	return 1;
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
	ShowDesk();                                   //界面
	
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
//	printf("debug i = %d\n", i);
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
//	printf("LogIn Successfully ! Welcome to Chat rOOm \n");
	printf("\033[33m\033[1mLogIn Successfully ! Welcome to Chat RooM  ___made by Dr.Lun\033[0m\n");  //发送者变色
	
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
	return 0;
}

