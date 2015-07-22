/*************************************************************************
    > File Name: 6.0my_ls.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月21日 星期二 11时06分45秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<pwd.h>	
#include<grp.h>
#include<sys/ioctl.h>
#include<termios.h>
#include<dirent.h>
#include<linux/limits.h>
#include<dirent.h>
#include<errno.h>

#define PARAM_NO 0
#define PARAM_A    1
#define PARAM_L     2

int GetCol()
{
	struct winsize size;
	ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
	return size.ws_col;
}

int MAXLEN; 
int maxlength;
int namelength;
int l_length;

void show_error(char * reason,  int line);
int  goto_dir(int flag_parameter, char * path);
void show_a_filename(char * name);
void show_file(int flag_parameter, char * path_name);

void show_error(char * reason,  int line)
{
	fprintf(stderr, "line :%d ", line);
	perror(reason);
	exit(1);
}

int goto_dir(int flag_parameter, char * path)	
{
	int i, j, len;
	DIR * dir;
	struct dirent * ptr;
//	struct stat buf;
	int count  = 0;
	char filename[256][PATH_MAX + 1];
	char temp[PATH_MAX];
		
/*	if((stat(path, &buf)) == -1)
	{
		perror("stat");
		return -1;	
	}
*/				
	if((dir = opendir(path)) == -1 )
	{
		perror("opendir");
		return -1;
	}  

	while((ptr = readdir(dir)) != NULL)       //获取该目录下文件总数和最长文件名
	{
		if((strlen(ptr ->d_name))> maxlength)
			//maxlength = ptr->d_reclen;
			maxlength = strlen(ptr ->d_name);
		count++;
		//printf("count = %d ", count);
	}	
	//此处需要closedir
	closedir(dir);
	
	if(count > 256)
	{
		printf("too many files here\n");
		exit(1);
	}
	
	if((dir = opendir(path)) == -1 )
	{
		perror("opendir");
		return -1;
	}
	
	len = strlen(path);
	
	for(i = 0; i < count ; i ++)      //获取文件所在位置，文件名，一起放入filename中
	{
		ptr = readdir(dir);
		if(ptr == NULL)
		{
			show_error("readdir", __LINE__);	
		}
		strncpy(filename[i], path, len); 
		filename[i][len] = 0;
		strcat(filename[i], ptr->d_name);
		filename[i][len + strlen(ptr->d_name)] = 0;	
		//printf("debug:filename:%s", filename[i]);
	}
	
	for(i = 0; i < count -1 ; i++)    
	{                                   //按文件名称排序
		for(j = 0; j < count - i - 1 ; j++)
		{
			if(strcmp (filename[j], filename[j+1]) > 0)
			{				
				strcpy(temp,filename[j+1]);
				temp[strlen(filename[j+1])]='\0';
				strcpy(filename[j+1],filename[j]);
				filename[j+1][strlen(filename[j])]='\0';
				strcpy(filename[j],temp);
				filename[j][strlen(temp)]='\0';
			}
		}
	}
		//printf("debug1a:filename:%s", filename[3]);

	
			
	for(i = 0; i < count ; i++)
	{
		show_file(flag_parameter, filename[i]);
	}
	printf("\n");

	closedir(dir);
}

void show_file(int flag_parameter, char * path_name)
{
	int i, j;
	struct stat buf;
	char name[NAME_MAX + 1];
	
	//从路径中解析出文件名；
	for(i = 0, j = 0; i < strlen(path_name); i ++)
	{
		if(path_name[i]  == '/')
		{
			j = 0;
			continue;
		}
		name[j++] = path_name[i];
	}

	//printf("debug %s",name);
	name[j] = 0;
	
	if(lstat(path_name, &buf) == -1)
	{
		show_error("stat", __LINE__);	
	}
	
	switch(flag_parameter)
	{
		case PARAM_NO:
			if(name[0] != '.')
			{
			//	printf("debug:filename[0]: %c", name[0]);
				show_a_filename(name);
	//		printf("debug ===-=========%d ", maxlength);
			}
			break;			
			
			
		case PARAM_A:
		
			show_a_filename(name);			
			break;
						
		case PARAM_L:
			//printf("debug III");
			show_file_details(buf, name);
			printf("%-s\n", name);	
			//printf("debug III");		
			break;
		
		case PARAM_A + PARAM_L:
		
			show_file_details(buf, name);
			printf("%-s\n", name);			
			break;
			
		default:			
			break;
			
			
	} 
}

void show_a_filename(char * name)
{	
	int i,len;
	
	if(l_length < maxlength)
	{
		printf("\n");	
		l_length = GetCol();
	}
	
	len  = strlen(name);
	len = maxlength - len;
	printf("%-s", name);
	//printf("debug ===-=========%d ", maxlength);
	
	for(i = 0; i < len; i++)
	{
		printf(" ");
	}
	printf("  ");
	l_length -= (maxlength + 2);
}

void show_file_details(struct stat buf, char * name)
{
	char buf_time[32];
	struct passwd * psd;
	struct group  * grp;
	
	if(S_ISLNK(buf.st_mode))
	{
		printf("l");
	}
	
	else if(S_ISREG(buf.st_mode))
	{
		printf("-");
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
	
	//--------------------------------------------------
	
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
//-------------------------------------------	
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
//---------------------------------------------------	
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
		printf("r");
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
	
	printf(" ");
//-----------------------------------------
	
	psd = getpwuid(buf.st_uid);
	grp  = getgrgid(buf.st_gid);
	printf("%-3d ",  buf.st_nlink);
	printf("%-8s", psd->pw_name);
	printf("%-8s", grp->gr_name);
	
	printf("%6d", buf.st_size);
	strcpy(buf_time, ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1]='\0'; //去掉换行符
	printf(" %s ", buf_time);

}



int main(int argc , char ** argv)
{
	int i, j,k, num = 0;
	char parameter[32];                  //参数
	char flag_parameter = 0;
	char path[PATH_MAX + 1];
	struct stat buf;

	
	l_length = GetCol();
	
	for(i = 0; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			for(k = 1, j = 0; k < strlen(argv[i]); k++, j++)
			{
				parameter[j] = argv[i][k];
			}
			num ++;
		}
	}
	
	for(i = 0; i < j; i++)
	{
		if(parameter[i] == 'a')
		{
			flag_parameter  |=  PARAM_A;   //或等于
			continue;
		}
		
		else if(parameter[i] == 'l')
		{
			flag_parameter |=  PARAM_L;  
			continue;
		}
		
		else
		{
			printf("my_ls : invalid option  %-c \n", parameter[i]);
			exit(1);
		}		
	}
	parameter[j] = 0;
	
	if((num + 1) == argc)
	{
		strncpy(path , "./" , 2);
		path[2] = 0;
		goto_dir(flag_parameter, path);
		return 0;
	}
	i = 1;
	
	do
	{
		if(argv[i][0] == '-')
		{
			i++;
			continue;
		}
		else
		{
			strcpy(path, argv[i]);
		
		
			if(stat(path, &buf) == -1)
			{
				show_error("stat", __LINE__);
			}
		
			if(S_ISDIR(buf.st_mode))
			{
				if(path[strlen(argv[i]) - 1] != '/')
				{
					path[strlen(argv[i]) ] = '/';
					path[strlen(argv[i]) + 1] = 0;
				}
				else
					path[strlen(argv[i])] = 0;
			
				goto_dir(flag_parameter, path);
				i++;		
			}
		
			else
			{
				show_file(flag_parameter, path);
				i++;
			}
		}
	}while(i < argc);

	return 0;
}










