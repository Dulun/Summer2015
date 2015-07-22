#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include <Linux/limits.hname)>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <error.h>
#define PARAM_NONE	0	//无参数
#define PARAM_A	1	//-a;显示所有文件
#define PARAM_L 2	//-l;一行只显示一个文件的详细信息 
#define PARAM_I 4       //-i:显示i节点
#define PARAM_G 8	//-i:显示文件信息，去掉
#define PARAM_R 20	//按字母排序的逆序输出
#define PARAM_T  33	//按修改时间排序
#define MAXROWLEN	80  //一行显示的最多字符数

int g_leave_len=MAXROWLEN;	//一行剩余长度,用于输出对齐
int g_maxlen;		//存放在某目录下最长文件名的长度
/*错误处理函数,打印出错误所在行的行数和错误信息*/
void my_err(const char *err_string,int line)
{
	fprintf(stderr,"line:%d ",line);
	perror(err_string);
	exit(1);
}
/*获取文件属性并打印*/
void display_attribute(struct stat buf,char * name,int a,int b)
{
	char buf_time[32];
	struct passwd   *psd;  //从该结构体中获取文件所有者的用户名
	struct group    *grp;	//从该结构体中获取文件所属组的组名
/*获取并打印文件类型*/
if(b==1)
{
	printf("%7d  ",buf.st_ino);
}	
if(S_ISLNK(buf.st_mode))	
{
	printf("l");
}
else if(S_ISREG(buf.st_mode))
{
	printf("_");
}
else if(S_ISDIR(buf.st_mode))
{
	printf("d");
}
else if(S_ISCHR(buf.st_mode))
{
	printf("c");
}
else if(S_ISBLK(buf.st_mode))
{
	printf("b");
}
else if(S_ISFIFO(buf.st_mode))
{
	printf("f");
}
else if(S_ISSOCK(buf.st_mode))
{
	printf("s");
}
/*获取并打印文件所有者的权限*/
if(buf.st_mode & S_IRUSR)
{
	printf("r");
}
else
{
	printf("-");
}
if(buf.st_mode & S_IWUSR)
{
	printf("w");
}
else
{
	printf("-");
}
if(buf.st_mode & S_IXUSR)
{	
	printf("x");
}
else
{
	printf("-");
}
/*获取并打印文件所有者同组的用户对该文件的操作权限*/
if(buf.st_mode & S_IRGRP)
{
	printf("r");
}
else
{
	printf("-");
}
if(buf.st_mode & S_IWGRP)
{
	printf("w");
}
else
{
	printf("-");
}
if(buf.st_mode & S_IXGRP)
{
	printf("x");
}
else
{
	printf("-");
}
/*获取并打印其它用户对该文件的操作权限*/
if(buf.st_mode & S_IROTH)
{
	printf("r");
}
else
{
	printf("-");
}
if(buf.st_mode & S_IWOTH)
{
	printf("w");
}
else
{			
	printf("-");
}
if(buf.st_mode & S_IXOTH)
{
	printf("x");
}
else
{
	printf("-");
}

printf("   ");
/*根据uid和gid获取所有者的用户与组名*/
psd=getpwuid(buf.st_uid);
grp=getgrgid(buf.st_gid);
printf("%4d ",buf.st_nlink);	//打印文件的链接数
printf("%-8s",psd->pw_name);
if(a==1)
{
 printf("%-8s",grp->gr_name);
}

printf("%6d",buf.st_size);	//打印文件的大小
strcpy(buf_time,ctime(&buf.st_mtime));
buf_time[strlen(buf_time)-1]='\0'; //去掉换行符
printf(" %s",buf_time);			//打印文件的时间信息
}
/*在使用-i选项时,打印名字,打印节点*/
void display_ino_t(struct stat buf,char *name) 
{
	//如果本行不足打印一个文件名则换行
	printf("%7d\n",buf.st_ino);
}
	   
	  
/*在没有使用-l选项,打印一个文件名,打印上下行对齐*/
void display_single(char *name)
{
	int i,len;
	//如果本行不足以打印一个文件名则换行
	 if(g_leave_len<g_maxlen)
	{
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
	len=strlen(name);
	len=g_maxlen-len;
	printf("%-s",name);
	for(i=0;i<len;i++)
	{
		printf(" ");
	}
		printf(" ");
		//下面的2指示空两格
	g_leave_len-=(g_maxlen+2);
	
}
/*
 *根据命令行参数的完整路径显示目标文件
 *参数flag:命令行参数
 *参数pathname:包含了文件的路径名
 */
void display(int flag,char *pathname)
{
	int i,j;
	struct stat  buf;
	char name[NAME_MAX + 1];
	/*从路径中解析出文件名*/
	for(i=0,j=0;i<strlen(pathname);i++)
	{
		if(pathname[i]=='/')
		{
			j=0;
			continue;
		}
		name[j++] =pathname[i];
	}
	name[j++]='\0';
	//printf("\n\n\n\%s\n\n\n\n",name);
/*用lstat而不是stat以方便解析链接文件*/
if(lstat(pathname,&buf)==-1)
{
	my_err("stat",__LINE__);
}
switch(flag)
{
	case PARAM_NONE:  //没有选项
		if(name[0]!='.')
		{
			display_single(name);
		}
			break;
	case PARAM_A: //-a:显示包括隐藏文件在内的所有文件
		display_single(name);
		break;
	case PARAM_L:	//-l:每个文件单独占一行,显示文件的详细属性信息
		if(name[0]!='.')
		{
		   display_attribute(buf,name,1,0);
		   printf(" %-s\n",name);
		 }
		break;
	case PARAM_I: //-i:显示文件的i节点
		if(name[0]!='.')
		{
			display_ino_t(buf,name);
			printf("%s\t",name);
		}
		break;
	case PARAM_G:
		if(name[0]!='.')
		{
			display_attribute(buf,name,0,0);
			printf("%-s\n",name);
		}
		break;
	case PARAM_R:
		if(name[0]!='.')
		{
			display_single(name);
		}
		break;	
	case PARAM_R + PARAM_L:			//同时有r和l的情况
		if(name[0]!='.')	
		{
			display_attribute(buf,name,1,0);
			printf(" %-s\n",name);
		}
		break;
	case PARAM_A + PARAM_R:			//同时有a和r的情况
			display_attribute(buf,name,1,0);
			printf(" %-s\n",name);
			break;
	case PARAM_R + PARAM_G:			//同时有r和g的情况
		if(name[0]!='.')
		{
			display_attribute(buf,name,0,0);
			printf(" %-s\n",name);
		}
			break;
	case PARAM_R + PARAM_I:		//同时有r和i的情况
		if(name[0]!='.')
		{
			display_ino_t(buf,name);
			printf("%s\n",name);
		}
		break;
	case PARAM_I + PARAM_L: //同时有ｉ和l的情况
		if(name[0]!='.')
		{
			display_attribute(buf,name,1,1);
			printf("%-s\n",name);
		}
		break;
	case PARAM_I +PARAM_L +PARAM_A:			//同时有i和l和a的情况
			display_attribute(buf,name,1,1);
			printf("%-s\n",name);
			break;
	case PARAM_I + PARAM_G:		//同时有i和Ｇ的情况
		if(name[0]!='.')
		{
			display_attribute(buf,name,0,1);
			printf("%-s\n",name);
		}
		break;
	case PARAM_I + PARAM_G + PARAM_A:		//同时有i,g,a的情况
			display_attribute(buf,name,0,1);
			printf("%-s\n",name);
			break;		
	case PARAM_A+PARAM_L: //同时有-a和-l选项的情况
		display_attribute(buf,name,1,0);
		printf(" %-s\n",name);
		break;
	default:
		break;
	}
}
void display_dir(int flag_param,char *path)
{
	DIR	*dir;
	struct dirent	*ptr;
	int count=0;
	char filenames[256][PATH_MAX+1],temp[PATH_MAX+1];
	//获取该目录下的文件总数和最长的文件名
	dir=opendir(path);
	if(dir == NULL)
	{
		my_err("opendir",__LINE__);
	}
	while((ptr=readdir(dir))!=NULL)
	{
		if(g_maxlen< strlen(ptr->d_name))
			g_maxlen=strlen(ptr->d_name);
		count++;
	}
	closedir(dir);
	if(count>256)
	{
		my_err("too many file under this dir",__LINE__);
	}
	int i,j,len=strlen(path);
	//获取目录下的所有文件名
	dir=opendir(path);
	for(i=0;i<count;i++)
	{
		ptr=readdir(dir);
		if(ptr==NULL)
		{
			my_err("readdir",__LINE__);
		}
		strncpy(filenames[i],path,len);
		filenames[i][len]='\0';
		strcat(filenames[i],ptr->d_name);
		filenames[i][len+strlen(ptr->d_name)] ='\0';
	}
	//使用冒泡排序法对文件名进行排序,排序后按字母顺序存储于filename
	for(i=0;i<count-1;i++)
		for(j=0;j<count-1-i;j++)
		{
			if(strcmp(filenames[j],filenames[j+1])>0)
			{
				strcpy(temp,filenames[j+1]);
				temp[strlen(filenames[j+1])]=='\0';
				strcpy(filenames[j+1],filenames[j]);
				filenames[j+1][strlen(filenames[j])]='\0';
				strcpy(filenames[j],temp);
				filenames[j][strlen(temp)]='\0';
			}
		}
	if((flag_param & PARAM_R)==0)		//如果命令中没有-r选项，
	{
	  for(i=0;i<count;i++)
	  {
		display(flag_param,filenames[i]);
	 }
	}
	else
	{
		for(i=count-1;i>=0;i--)
		{
			display(flag_param,filenames[i]);
		}
	}
	closedir(dir);
	//如果命令行中没有-l选项,打印一个换行符
	if((flag_param & PARAM_L)==0)
		printf("\n");
}
int main(int argc,char **argv)
{
	int i,j,k,num;
	char path[PATH_MAX+1];
	char param[32];//保存命令行参数,目标文件名不在此列
	int  flag_param=PARAM_NONE; //参数种类;
	struct  stat buf;
/*命令行参数的解析,.....*/
j=0,
num=0;
for(i=1;i<argc;i++)
{
	if(argv[i][0]=='-')
	{
		for(k=1;k<strlen(argv[i]);k++,j++)
		{
			param[j]=argv[i][k];    //获取-后面的参数保存到数组param中
		}
		num++; //保存"-"的个数
	}
}
/*只支持参数a和l,如果含有其它选项就报错*/
for(i=0;i<j;i++)
{
	if(param[i]=='a')
	{
		flag_param |= PARAM_A;
		continue;
	}
	else if(param[i]=='l')
	{
		flag_param |= PARAM_L;
		continue;
	}
	else if(param[i]=='i')
	{
		flag_param |= PARAM_I;
		continue;
	}
	else if(param[i]=='g')
	{
		flag_param |=PARAM_G;
		continue;
	}
	else if(param[i]=='r')
	{
		flag_param |= PARAM_R;
		continue;
	}
	else
	{
		printf("my_ls:invalid option -%c\n",param[i]);
		exit(1);
	}
}
param[j]='\0';
//如果没有输入文件或目录,就显示当前目录
if((num+1)==argc)
{
	strcpy(path,"./");
	path[2]='\0';
	display_dir(flag_param,path);
	return 0;
}
i=1;
do{
	//如果不是目标文件或目录,解析下一个命令行参数
	if(argv[i][0]=='-')
	{
		i++;
		continue;
	}
	else
	{
		strcpy(path,argv[i]);
		//如果不是目标文件或目录,解析下一个命令行参数
		if(stat(path,&buf)==-1)
		{
			my_err("stat",__LINE__);
		}
		if(S_ISDIR(buf.st_mode))	//argv[i]是一个目录
		// 如果目录的最后一个字符不是'/',就加上'/'
		{
			if(path[strlen(argv[i])-1]!='/')
			{
				path[strlen(argv[i])] |='/';
				path[strlen(argv[i])+1] |='\0';
			}
			else
			{
				path[strlen(argv[i])]='\0';
			}
			display_dir(flag_param,path);
			i++;
		}
		else		//argv[i]是一个文件
		{
			display(flag_param,path);
			i++;
		}
	 }
	}while(i<argc);
	return 0;
}
			
