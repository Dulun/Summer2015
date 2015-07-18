/*************************************************************************
    > File Name: b.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月03日 星期五 14时00分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <string.h>
#include <termios.h>  
#include <unistd.h>  
//#include <ncurses.h>
//#include <windows.h>
//#include <io.h>

#define LEN  sizeof(struct Node)
#define LEN1 sizeof(struct FileName)

struct Node * Creat        ();							//建表无头节点，返回头指针
struct Node * Sort         (struct Node *head);				//链表冒泡
struct Node * Reverse      (struct Node *head);			//逆置链表
struct Node * AddNodeBefore(struct Node * head);	   		//头插
struct Node * Read_info    ();						//读入信息
struct Node * creatNewList ();						//新增表

void Insert(struct Node * head);		                			//插入数据	
void Print (struct Node * head);					         //输出
void Remove(struct Node **head);					//删除
void AddNodeAfter (struct Node * head);				//尾插
void continueCreat(struct Node * head);                 			//继续尾插
void ChangeScore  (struct Node * head);				//更改成绩
int    Safe_info    (struct Node * head);					//保存信息
void Save_now_info(struct Node * head, char *filename); 	//保存到当前文件
int    CheckSumIfSame(struct Node * head, int num);        	//检测学号是否重复    
void ShowDesk     ();								//显示界面
void ChangePasswd ();                                   			//更改密码
int    SignIn       ();                                   				//登陆

struct Node
{
	int num;
	int score;
	struct Node * next;
};

struct FileName
{
	char filename[20];
	struct FileName * next;
};

struct Node * Creat()
{
	struct Node *p,*q,*head=0;
	int num,score;
//	int flag;

	printf("input num:");
	scanf("%d",&num);
	fflush(stdin);
	printf("input score:");
	scanf("%d",&score);
	fflush(stdin);
	while(num)
	{
//		flag = checkSumIfSame(head, num);
		if(CheckSumIfSame(head, num) != 0)
		{
			printf("提示：您刚输入的学号与之前的重复，信息作废， 请重新输入\n");
			printf("input num:");
			scanf("%d",&num);
			fflush(stdin);
			printf("input score:");
			scanf("%d",&score);
			fflush(stdin);
			continue;
		}

		p = malloc(LEN);

		if(head == 0)              //链入链表
			head = p;
		else
			q ->next = p;

		p ->num  = num;
		p ->score = score;
		p ->next = 0;
		q = p;

		printf("input num:");
		scanf("%d",&num);
		fflush(stdin);
		printf("input score:");
		scanf("%d",&score);
		fflush(stdin);
	}

	return head;
}



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

int CheckSumIfSame(struct Node * head, int num)
{
	struct Node * p;
	int flag = 1;

	for(p = head; p && p->num != num; p = p->next)
		;
	if(p == NULL)
		flag = 0;

	return flag;
}

void Print(struct Node * head)
{
	struct Node * p;

	if(head == NULL)
	{
		printf("                当前表中无信息！\n");
		return;
	}

	for(p = head; p; p = p-> next)
		printf("         num: %d   \t\tscore: %d\n",p ->num,p ->score);
}

struct Node * Sort(struct Node *head)
{
	struct Node * p,*p1,*p2 ,* end;

	if(head == NULL)
	{
		printf("                当前表中无信息！\n");
		return NULL;
	}

	p1 = malloc(LEN);
	p1->next = head;
	head = p1;

	for(end = 0; end != head; end = p)
		for(p = p1 = head; p1 ->next->next!= end ;p1 = p1 ->next)
		{//交换
			if(p1 ->next->score < p1 ->next->next->score)
			{
				p2 = p1->next->next;
				p1 ->next ->next = p2->next;
				p2->next = p1 ->next;
				p1 ->next = p2;
				p = p1 ->next ->next;
			}
		}
	p1 = head;
	head = head->next;
	free(p1);
	p1 =0;

	return head;
}

void continueCreat(struct Node * head)
{
	struct Node *p,*q,*hp=0;
	int num,score;

	if(head == NULL)
	{
		printf("当前表中无信息！");
		return;
	}

	for(hp = head; hp->next; hp = hp->next)  //找到最后节点
		;

	printf("input num:");
	scanf("%d",&num);
	fflush(stdin);
	printf("input score:");
	scanf("%d",&score);
	fflush(stdin);
	while(num)
	{
		if(CheckSumIfSame(head, num) != 0)
		{
			printf("提示：您刚输入的学号与之前的重复，信息作废， 请重新输入\n");
			printf("input num:");
			scanf("%d",&num);
			fflush(stdin);
			printf("input score:");
			scanf("%d",&score);
			fflush(stdin);
			continue;
		}

		p = malloc(LEN);
		if(hp ->next == 0)
			hp ->next = p;
		else
			q ->next = p;

		p ->num  = num;
		p ->score = score;
		p ->next = 0;
		q = p;
		printf("input num:");
		scanf("%d",&num);
		fflush(stdin);
		printf("input score:");
		scanf("%d",&score);
		fflush(stdin);
	}
	printf("\n录入成功\n");
}

void Remove(struct Node ** head)
{
	struct Node *p , *q, *t;
	int num;

	if(*head == NULL)
		return;

	p =malloc(LEN);
	p ->num   = -1;
	p ->score = -1;
	p ->next  = *head;
	*head = p;
	q = *head;

	printf("input the num need to be removed:"); 
	scanf("%d",&num);
	for(p = q = *head; p && p ->num != num; q = p, p = p->next)                                                                     //找
		;
	t = p;
	if(p)	
	{
		q ->next = p->next;
		p ->next = NULL;
		free(t); //释放
		t = 0;
	}

	else
	{
		printf("Can not find it\n");
		getch();	
	}
	
	p = *head;
	*head = (*head)->next;   //更改
	free(p);
	p = 0;
}

void ChangeScore(struct Node *head)
{
	struct Node *p;
	int num;

	if(head == NULL)
		return;
	Print(head);

	printf("请输入需要被修改学生的学号："); 
	scanf("%d",&num);

	for(p =head; p && p ->num != num; p = p->next)                                                                     //找
		;
	if(p)	
	{
		printf("\n\n请输入新成绩：");
		scanf("%d", &p ->score);
		system("clear");
		printf("\n\n修改成功\n\n");
		Print(head);
	}
	else
	{
		printf("没有找到该学生\n");
		getch();
	}
}

struct Node * Reverse(struct Node *head)
{
	struct Node *p, *p1 = 0, *p2 = head;

	if(head == NULL)
	{
		printf("                当前表中无信息！\n");
		return NULL;
	}

	while(p2 != 0)
	{
		p = p2 ->next;
		p2 ->next = p1;
		p1 = p2;
		p2 = p;
	}
	head = p1; 

	return head ;
}

void Insert(struct Node * head)
{
	struct Node *p,*q;
	int n;
	int num;
	int score;
	int flag;

	if(head == NULL)
		return;

	printf("input num;(insert):");
	scanf("%d",&n);

	for(q = head; q && q ->num != n; q = q ->next)
		;

	printf("input num:");
	scanf("%d",&num);
	fflush(stdin);
	printf("input score:");
	scanf("%d",&score);
	fflush(stdin);
	flag = CheckSumIfSame(head, num);

		while(flag != 0)
		{
			printf("提示：您刚输入的学号与之前的重复，信息作废， 请重新输入\n");
			printf("input num:");
			scanf("%d",&num);
			fflush(stdin);
			printf("input score:");
			scanf("%d",&score);
			fflush(stdin);
			flag = CheckSumIfSame(head, num);			
		}

	if(q)
	{
		p = q ->next;
		q -> next = malloc(LEN);
		q ->next->num   = num;
		q ->next->score = score;
		//printf("input new num:");
		//scanf("%d",&q ->next->num);
	//	printf("input new score:");
	//	scanf("%d",&q ->next->score);
		q->next ->next = p;
	}

	else
	{
		printf("cannot find the num\n");
	}
}

struct Node * AddNodeBefore(struct Node * head)        //头插
{
	struct Node * p;
	int flag, flag1;
	int num;
	int score;
	
	if(head == NULL)
	{
		printf("                当前表中无信息！\n");
		return NULL;
	}
	
	p = head ->next;
	p = malloc(LEN);
	printf("input new num(before first):");
	scanf("%d",&num);

	flag = CheckSumIfSame(head, num);

		while(flag != 0)
		{
			printf("提示：您刚输入的学号与之前的重复，信息作废， 请重新输入\n");
			printf("input num:");
			scanf("%d",&num);
			fflush(stdin);
			printf("input score:");
			scanf("%d",&score);
			fflush(stdin);
			flag = CheckSumIfSame(head, num);	
			flag1 = 1;
		}

	p ->num = num;
	
	if(flag1 == 1)
	{
		p ->score = score;
		p ->next = head;
		head = p;
		return head;
	}
	printf("input new score(before first):");
	scanf("%d",&score);
	p ->score = score;
	p ->next = head;
	head = p;

	return head;
}

void AddNodeAfter(struct Node * head)
{
	struct Node * p,*q;
	int flag, flag1;
	int num;
	int score;


	if(head == NULL)
	{
		printf("                当前表中无信息！\n");
		return;
	}
	
	for(p = head; p ->next; p = p ->next)
		;	

	q = malloc(LEN);
	p ->next = q;
	q ->next = 0;
	printf("input new num(after last):");
	scanf("%d",&num);

	flag = CheckSumIfSame(head, num);
		while(flag != 0)
		{
			printf("提示：您刚输入的学号与之前的重复，请重新输入\n");
			printf("input num:");
			scanf("%d",&num);
			fflush(stdin);
			printf("input score:");
			scanf("%d",&score);
			fflush(stdin);
			flag = CheckSumIfSame(head, num);
			flag1 = 1;
		}

	q ->num = num;
	if(flag1 == 1)
	{
		q ->score = score;
		return;
	}

	printf("input new score(after last):");

	q ->score = score;
}

void Save_now_info(struct Node * head, char *filename)
{
	struct Node * stu;
	FILE * fp;

	if((fp = fopen(filename, "wt")) == NULL)
	{
//		printf("%s", filename); //调试
		printf("文件出错！按任意键退出\n");
		getch();
		return;	
	}

	for(stu = head; stu != NULL; stu = stu -> next)
	{
		fwrite(stu, LEN, 1,fp);		
	}
	fclose(fp);
}

int Save_info(struct Node * head)
{
	struct Node * stu;
	FILE * fp, *fpp;
	FILE * list;
	char filename[40];
	char exit;
	int flag = 0;

	list = fopen("fileName.txt", "at+");

	if(head == NULL)
		return 0;

	printf("请输入班级：");
	scanf("%s", filename);

	fpp = fopen(filename, "r");
	if(fpp != NULL)
	{		
		printf("该班级已存在， 是否覆盖？（Y/N）");
		flag = 1;
		exit = getch();
		if(exit == 'n' || exit == 'N' )
			return -1;
	}

	if(flag == 0)
	{
		fprintf(list, "%s ", filename);
		fclose(list);
	}

	if((fp = fopen(filename, "wt")) == NULL)
	{
		printf("文件出错！按任意键退出\n");
		getch();
		return 0;		
	}

	for(stu = head; stu != NULL; stu = stu -> next)
	{
		fwrite(stu, LEN, 1,fp);		
	}
	fclose(fp);

	return 1;
}

struct Node * Read_info(char * fileName)
{
	struct Node * head, *r, *stu;
	struct FileName *filehead, *file1, *file2;
	FILE * fp;
	FILE * list;
	char filename[40];

	if((list = fopen("fileName.txt", "rt")) == NULL)
	{
		printf("目前还未录入任何班级的信息， 按任意键退出\n");
		getch();
		return NULL;
	}
	filehead = malloc(LEN1);
	filehead ->next = NULL;

	for(file1 = filehead; ; file1 -> next = file2, file1 = file2)
	{//读入
		file2 = (struct FileName *)malloc(LEN1);
		fscanf(list,"%s", file2 -> filename);

		if(feof(list))
		{
			file1 -> next = file2;
			file2 ->next = NULL;
			fclose(list);
			break;
		}
	}

	list = fopen("fileName.txt", "rt");
	printf("已经录入的班级有：\n");

	for(file1 = filehead ->next; file1->next != NULL; file1 = file1->next)
	{//输出
		printf("%s", file1->filename);		
		printf("\n");
	}

	printf("\n\n请输入要打开的班级");
	scanf("%s",filename);
	strcpy(fileName, filename);
	fp = fopen(filename,"rt");

	if(fp  == NULL)
	{
		system("clear");
		printf("\n\n\n您还未录入该班级成绩信息\n");
		return NULL;
	}

	head  = (struct Node *)malloc(LEN);
	head ->next = NULL;
	
	for(r = head; ;  r -> next = stu, r = stu)
	{
		stu = (struct Node *)malloc(LEN);
		fread(stu, LEN, 1, fp);
		if(feof(fp))
			break;
	}

	r ->next = NULL;
	fclose(fp);
	system("clear");
	printf("         班级：%s  载入完毕 成绩为：\n", filename);
	head = head ->next;

	return head;
}

void ReturnDesk() 
{
	printf("\n\n\n\n\n\n按任意键回到主界面\n");
	getch();
	getch();

	return ;
}

struct Node * InitLink()
{
	int choose;
	struct Node * head;

	system("clear");
	printf("\n\n\n\n");
	printf("             ****************   1.建立新成绩单     ****************\n\n\r");
	printf("             ****************   2.进入系统         ****************\n\n\n\n\n");
    	printf("             ****************   请输入选项：             ****************");
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

	scanf("%d", &choose);
	if(choose == 1)
	{
		printf("录入学生信息，请输入数据（学号-成绩）\n");
		head = Creat();
		printf("录入成功，数据为：\n");
		Print(head);
		return head;
	}

	if(choose == 2)
	{
		return 0;
	}

	return 0;      //
}

int SignIn()
{
	int i,keycount, flag = 0;
	char Initpass[20];
	char Initpassagain[20];
	char pass[20];
	char NowPass[20];
	FILE * fp;
	
	fp = fopen("passwd", "rt");
	system("clear");

	if(fp == NULL)
	{
		do  //无密码文件, 首次使用
		{		
			printf("\n\n\n\n");
			printf("         ****************      欢迎使用学生成绩管理系统              ****************\n");
			printf("\n\n\n\n");
			printf("         ****************      初次使用请首先初始化密码:             ****************");
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		
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
			printf("\n");
			printf("         ****************         请再次输入初始密码:                ****************");
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

			for(keycount = 0; keycount < 20; keycount++)
			{
				Initpassagain[keycount] = getch();

				if(Initpassagain[keycount] == 127)
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

				if(Initpassagain[keycount] == 13)    
				{                        //回车结束
					Initpassagain[keycount] = 0;
					break;
				}
				printf("*");
			}//for(keycount = 0; keycount < 20; keycount++)

			if(strcmp(Initpass, Initpassagain) != 0)
			{
				system("clear");
				printf("\n\n\r             两次输入的密码不一致， 3s后请重新输入\n");
				sleep(3);
				system("clear");
				fflush(stdin);
			}
			else
			{				
				fp = fopen("passwd", "wt");
				fwrite(Initpass,1, 20, fp);
				system("clear");
				printf("\n\n\n\n\r     初始化密码成功， 请牢记密码， 如若忘记请联系管理员\n\r");
				printf("\n\n\n\n\r                    2s后重新进入\n");
				fclose(fp);
				sleep(2);
			}//else

		}while(strcmp(Initpass, Initpassagain) != 0); //do while
	}//if(fp == NULL)

	system("clear");
	fflush(stdin);

	fp = fopen("passwd", "rt");
 	fread(NowPass,1, 20, fp);

	printf("\n\n\n\n\r");
	printf("     ****************      欢迎使用学生成绩管理系统        ****************\n");
	printf("\n\n\n\n\r");
	printf("     ****************      请输入密码:                ****************");
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

	for(i = 0; i < 3; i++)	
	{
		for(keycount = 0; keycount < 20; keycount++)   
		{                             //输入密码隐式用***代替
			fflush(stdin);
			pass[keycount] = getch();

			if(pass[keycount] == 127)
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

			if(pass[keycount] == 13)    
			{                        //回车结束
				pass[keycount] = 0;
				break;
			}
			printf("*");
		}//	for(keycount = 0; keycount < 20; keycount++)
	
		if(strcmp(pass, NowPass) == 0)
		{
			system("clear");
			printf("密码正确\n");
			flag = 1;
			break;
		}//if
		if(strcmp(pass, NowPass) != 0)
		{
			system("clear");
			printf("     ****************      密码错误， 请重新输入！    ****************");
			printf("\n\n\n\n\r");
			printf("     ****************      请输入密码:                ****************");
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
			pass[0] = 0;
			fflush(stdin);				
		}//if
	}//	for(i = 0; i < 3; i++)

	if(flag == 0)
	{
		system("clear");
		printf("\n\n\n\n\n\n\r        对不起， 您输入的密码有误超过3次，请稍后再试\n\n");
		printf("\n\n\n\n\n\n\r               谢谢使用， ----made by Mr.Du 丶Lun\n\n\n");
		getch();
		exit(1);
	}

	return flag;
}

void ChangePasswd()
{
	FILE * fp;
	char OldPass[20];
	char confirmOldPass[20];
	char NewPass[20];
	char confirmNewPass[20];
	char keycount;
	int flag = 0;

	fflush(stdin);
	fp = fopen("passwd", "rt");
	fread(confirmOldPass, 1, 20, fp);
	fclose(fp);

	printf("     ****************      请输入旧密码:                ****************");
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	getch(); //


	for(keycount = 0; keycount < 20; keycount++)
	{
		fflush(stdin);
		OldPass[keycount] = getch();

		if(OldPass[keycount] == 127)
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
		if(OldPass[keycount] == 13)    
		{                        //回车结束
			OldPass[keycount] = 0;
			break;
		}
		printf("*");
	}

	if(strcmp(OldPass, confirmOldPass) == 0)
	{
		while(flag == 0)
		{
	
			printf("\n\r     ****************      请输入新密码:                ****************");
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

		for(keycount = 0; keycount < 20; keycount++)
		{
			NewPass[keycount] = getch();
			if(NewPass[keycount] == 127)
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
			if(NewPass[keycount] == 13)    
			{                        //回车结束
				NewPass[keycount] = 0;
				break;
			}
				printf("*");
		}

	//	printf("\n################请再次输入新密码:");
			printf("\n\r     ****************    请再次输入新密码:                ****************");
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		for(keycount = 0; keycount < 20; keycount++)
		{
			confirmNewPass[keycount] = getch();
			if(confirmNewPass[keycount] == 127)
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
			if(confirmNewPass[keycount] == 13)    
			{                        //回车结束
				confirmNewPass[keycount] = 0;				
				break;
			}
			printf("*");
		}

		if(strcmp(NewPass, confirmNewPass) != 0)
		{
			system("clear");
			printf("\n\n              两次输入的密码不一致， 3s后请重新输入\n");
			sleep(3);
			system("clear");
			fflush(stdin);
		}
		else
		{
			system("clear");
			printf("\n\n\n      密码修改成功， 请牢记密码!  如若忘记请联系管理员");
			flag = 1;
		}

		fp = fopen("passwd", "wt");
		fwrite(NewPass, 20, 1, fp);	
		fclose(fp);
		}//while
	}
	else
	{
		printf("\n\n\n                              密码错误！\n");
	}
}

struct Node * creatNewList()
{
		struct Node * head;

		printf("录入学生信息，请输入数据（学号-成绩）\n");
		head = Creat();
		printf("录入成功，数据为：\n");
		Print(head);
		return head;
}

void FreeAllNodes(struct Node * head)
{
	struct Node * p, *q;

	for(p = q = head; q; p = q)
	{
		q = p ->next;
		free(p);
	}
}

void ShowDesk()
{
	printf("\n                     欢迎使用学生成绩管理系统                 \n\n\r");
	printf("     ****************   1. 查看当前信息            ****************\n\r");
	printf("     ****************   2. 继续录入                ****************\n\r");
	printf("     ****************   3. 新建成绩单              ****************\n\r");
	printf("     ****************   4. 当前成绩单按socre排序   ****************\n\r");
	printf("     ****************   5. 插入新成绩              ****************\n\r");
	printf("     ****************   6. 删除某学生信息          ****************\n\r");
	printf("     ****************   7. 修改某学生成绩          ****************\n\r");	
	printf("     ****************   8. 在最前面插入信息        ****************\n\r");
	printf("     ****************   9. 在最后面插入信息        ****************\n\r");
	printf("     ****************   10.倒序排列成绩信息        ****************\n\r");
	printf("     ****************   11.保存当前班级成绩单      ****************\n\r");
	printf("     ****************   12.查看已录入班级的成绩    ****************\n\r");
	printf("     ****************   13.更改密码                ****************\n\r");
    	printf("\n\n\r");
	printf("     --------------------0. 退出                      ----------------\n\r");
	printf("\n\n\n");
	printf("     ****************     请输入选项：             ****************");
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

int main()
{
	int n = -1;
	int flag = 0;
	int flag1;
	int save;
	struct Node * head;
	char fileName[20] = "0";

	flag = SignIn();           //登陆

	if(flag == 1)
	{
		head = InitLink();      //初始化
	}
	
	for(;;)  //各种功能 一直循环
	{
		fflush(stdin);				
		flag = 1;
		ReturnDesk();	

		system("clear");
		ShowDesk();
		fflush(stdin);
		scanf("%d", &n);
		fflush(stdin);	

		if(n == 0)
		{
			system("clear");
			FreeAllNodes(head);
			printf("\n\n\n\n\n\n               谢谢使用， ----made by Mr.Du 丶Lun\n\n\n\n\n\n");
			getch();
			exit(0);
		}//if

		if(n == 1)	
		{
			system("clear");
			Print(head);
		}//if
		if(n == 4)
		{
			system("clear");
			head = Sort(head);
			printf("sort over,new info:\n\n\n");
			Print(head);
		}
		if(n == 5)
		{
			system("clear");
			Print(head);
			Insert(head);
			system("clear");
			printf("insert over\n");
			Print(head);			
		}
		if(n == 6)
		{
			system("clear");
			Print(head);
			Remove(&head);
			system("clear");
			printf("remove over new info:\n");
			Print(head);
		}
		if(n == 8)
		{
			system("clear");
			Print(head);
			head =AddNodeBefore(head);
		}
		if(n == 9)
		{
			system("clear");
			Print(head);
			AddNodeAfter(head);
		}
		if(n == 10)
		{
			system("clear");
			printf("倒序排列为：\n");	
			head = Reverse(head);
			Print(head);
		}	
		if(n == 11)
		{
			system("clear");
			if(flag1 == -1)
			{
				Save_now_info(head, fileName);
				save = 1;
			}
			else
			{
				//printf("%d", flag);
				save = Save_info(head);
			}
			if(save == 1)
				printf("保存成功");
		}
		if(n == 12)
		{
			system("clear");
			head = Read_info(fileName);
			flag1 = -1;
			printf("\n");
			Print(head);
		}
		if(n == 2)
		{
			system("clear");
			Print(head);
			continueCreat(head);
		}
		if(n == 13)
		{
			system("clear");
			ChangePasswd();
			
		}
		if(n == 3)
		{
			system("clear");
			flag1 = 1;
			head = creatNewList();
		}
		if(n == 7)
		{
			system("clear");
			ChangeScore(head);
		}

	}//死循环
	
	getch();
}//main
