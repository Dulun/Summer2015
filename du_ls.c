/*************************************************************************
    > File Name: du_ls.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月23日 星期四 13时34分19秒
 ************************************************************************/


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
#define PARAM_A  1
#define PARAM_L  2
#define PARAM_R  8

#define SIMPLE       0
#define  DETAILS    1
#define HAVE          1
#define NOTHAVE   0

int flag;
int MAXCOL; 
int maxlength;
int namelength;
int l_length;
int judgeDir;
char * cwd;
void R();
char * get_now_path();
int GetMaxCol();
int JudgeDir(char * path);
void show_all_filename(char * path_name,int flag, int have_cd);
void show_error(char * reason,  int line);
int get_maxlength(char * path);
void show_error(char * reason,  int line);
int get_maxlength(char * path);
 void show_a_filename(char * name);
void show_a_file_details(char * path,char * name);

int l_path_d_name = 0;
char * path_R ;

char * get_now_path()
{	
	cwd = getcwd(0, 0);	
	return cwd;
}

int main(int argc, char **argv)
{
	char parameter[5];
        int ch;  
        int i;
        char * cwd;
        char flag = 0;
        //opterr = 0;  
        cwd = get_now_path(); 
        GetMaxCol();   
        l_length = MAXCOL;  
        
        while ((ch = getopt(argc, argv, "laR")) != -1)  
        {  	
        	flag = 1;
        
        	//printf(" debug : now the ch is %d",ch);
        	
        	if(ch == 'l')
        	{
        		printf("now the ch is %c",ch);
        		show_all_filename(cwd,DETAILS,NOTHAVE);
        	}
        	if(ch == 'a')
        	{	
        		show_all_filename(cwd,SIMPLE,HAVE);
        			
        	}    
        	if(ch == 'R')
       		{
       			path_R = cwd;
        		R();       		        		      		
       		} 
        	
        }
        
        if(flag == 0)
       	{
        	show_all_filename(cwd,SIMPLE, NOTHAVE);       		
       	} 
        
//        printf("now the ch is %d",ch);
        
       
      //  printf("opt%c undefined\n",optopt);  
}

void R()
{
	DIR * dir;
	struct dirent * ptr;
	int i, l_path_R;
	char * path_RR = path_R;
	
	printf("\npath:%s:\n", path_R);
	show_all_filename(cwd,SIMPLE, NOTHAVE); 
	if((dir = opendir(path_R)) == NULL)
	{
		show_error("open", __LINE__);	
	}
	
//	printf("debug : path : %s", path_R);
	
	while((ptr = readdir(dir)) != NULL)
	{
		if(flag == 1)
		{
			l_path_R = strlen(path_R);
			path_R[l_path_R - l_path_d_name  - 1 ] = '\0';
		}		
		
		flag = 5;	
		
		l_path_d_name = strlen(ptr -> d_name);
		l_path_R = strlen(path_R);
		//i = JudgeDir(ptr->d_name);
		path_RR[l_path_R] = '/';
		path_RR[l_path_R + 1] = '\0';			
		strcat(path_RR, ptr->d_name);
		if(ptr -> d_name[0] != '.')
		{
			i = JudgeDir(path_RR);
			printf("debug %d \n", i);
		}
		
		printf("\n R debug : d_name = %s path_R:%s\n", ptr->d_name, path_RR);		
	
		if(i == 1 && ptr -> d_name[0] != '.')
		{
			printf("\ndebug : NOW path_R = %s\n", path_R);
			l_path_d_name = strlen(ptr -> d_name);
			l_path_R = strlen(path_R);
			printf("\n R debug : l_path_d_name is %d   %d\n", l_path_d_name, l_path_R);
			
			path_R[l_path_R] = '/';
			path_R[l_path_R + 1] = '\0';			
			strcat(path_R, ptr->d_name);
			l_path_d_name = strlen(ptr -> d_name);
			l_path_R = strlen(path_R);
//			memcpy(path_R + l_path_R, ptr -> d_name, l_path_d_name);
			printf("\ndebug : NEW path_R = %s\n", path_R);			
			
			R();
		}		
	}
	flag = 1;	
//	printf("\ndebug:%s\n",path_R);		
}
 
int GetMaxCol()			
{
	struct winsize size;
	ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
	MAXCOL = size.ws_col;
}

int JudgeDir(char * path)
{
	DIR * dir;
	
	dir = opendir(path);
	if(dir == NULL)
	{
		return 0;
	}
	else
	{
		closedir(dir);
		return 1;
	}
}



void show_name_in_dir()
{
		
}

void show_all_filename(char * path_name,int flag, int have_cd)
{
	DIR * dir;
	struct dirent * ptr;
	struct stat buf;
	
	get_maxlength(path_name);

	if((dir = opendir(path_name)) == 0)
	{
		perror("opendir:");
		return -1;
	}
	
	if(have_cd == HAVE)
	{
		while((ptr = readdir(dir)) != NULL)
		{
			if(flag == SIMPLE)
				show_a_filename(ptr->d_name);
			if(flag == DETAILS)
				show_a_file_details(cwd,ptr->d_name);
		}
	}
	
	else
	{
		while((ptr = readdir(dir)) != NULL)
		{
			if(flag == SIMPLE && ptr ->d_name[0] != '.')
				show_a_filename(ptr->d_name);
			if(flag == DETAILS && ptr ->d_name[0] != '.')
				show_a_file_details(cwd,ptr->d_name);
		}
	}
	printf("\n");
	closedir(dir);
	return 0;
}


void show_error(char * reason,  int line)                      
{
	fprintf(stderr, "line :%d ", line);
	perror(reason);
	exit(1);
}

int get_maxlength(char * path)
{
	int i, j, len;
	DIR * dir;
	struct dirent * ptr; 
	struct stat buf;
	
	int count  = 0;
		
	if((stat(path, &buf)) == -1)
	{
		perror("stat");
		return -1;	
	}
				
	if((dir = opendir(path)) == -1 )
	{
		perror("opendir");
		return -1;
	}  
	
//	if((ptr = readdir(dir)) == NULL)
//		{
//				show_error("readdir", __LINE__);
//		}
	while((ptr = readdir(dir)) != NULL)       //获取该目录下文件总数和最长文件名
	{
		if((strlen(ptr ->d_name))> maxlength)
			//maxlength = ptr->d_reclen;
			maxlength = strlen(ptr ->d_name);
		count++;
		//printf("count = %d ", count);
	}	
	
//	printf("\ndebug: maxlength = %d\n");
	//此处需要closedir;
	closedir(dir);	
}
void show_a_filename(char * name)                                
{	
	int i,len;
	
	if(l_length < maxlength)
	{
		printf("\n");	
		l_length = MAXCOL;
	}
	
	len  = strlen(name);
	len = maxlength - len;
	
	judgeDir = JudgeDir(name);
	
	if(judgeDir == 0) 	
	{
		printf("%-s", name);
	//printf("debug ===-=========%d ", maxlength);
	}
	else      
	{
		printf("\033[35m%-s\033[0m", name);
	}
	
	for(i = 0; i < len; i++)
	{
		printf(" ");
	}
	printf("  ");
	l_length -= (maxlength + 2);
}


void show_a_file_details(char * path,char * name)
{	
	char buf_time[32];
	struct stat buf;
	DIR * dir;
	struct passwd * psd;
	struct group  * grp;
	
	lstat(name, &buf);

	if((dir = opendir(path)) == 0)
	{
		perror("opendir:");
		return -1;
	}
	
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
	printf(" %s\n", name);
}




/*
struct dir
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
				
	if((dir = opendir(path)) == -1 )
	{
		perror("opendir");
		return -1;
	}  
	
	if((ptr = readdir(dir) = NULL) == -1)
			{
				show_error("readdir", __LINE__);
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

	printf("debug goto to showfile: %s\n", filename[3]);
			
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
	int l;
	
	judgeDir = JudgeDir(path_name);
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
			if(name[0] != '.')
			{
				if(lstat(path_name, &buf) == -1)
				{
					show_error("stat", __LINE__);	
				}
				show_file_details(buf, name);
				ChangePrintColor(name);
				//printf("debug III");		
			}
			break;
		
		case PARAM_A + PARAM_L:

			if(lstat(path_name, &buf) == -1)
			{
				show_error("stat", __LINE__);	
			}
			
			show_file_details(buf, name);
			ChangePrintColor(name);
			break;

		case PARAM_R:

			if(name[0] != '.')
			{
				show_a_filename(name);
			}
//		d	if(name[0] != '.')
//			{
				if(judgeDir == 1 && name[0] != '.')
			//	if(judgeDir == 1)
				{	
					printf("\n\n./%s:\n", name);
					l = strlen(name);
					printf("\ndebug strlen = %d\n",l);
				//	name[4] = '/';
					name[l] = '/';
					name[l + 1] = '\0';
				//	name[4 + 1] = '\0';
					//printf("debug to gotodir: %s\n",name);
					goto_dir(flag_parameter,name);
					printf("\n");
				}

//			}
			break;

		case PARAM_R + PARAM_A:

			if(judgeDir == 1)
			{
				goto_dir(flag_parameter,name);		
			}

			show_a_filename(name);
			break;

		case PARAM_R + PARAM_L:

			if(judgeDir == 1)
			{
				goto_dir(flag_parameter,name);		
			}
			break;

		case PARAM_R + PARAM_L + PARAM_A:

	//		if(lstat(path_name, &buf) == -1)
	//		{
	//			show_error("stat", __LINE__);	
	//		}

			if(judgeDir == 1)
			{
				goto_dir(flag_parameter,name);		
			}

			show_file_details(buf, name);
			ChangePrintColor(name);
			break;
			
		default:			
			break;
			
			
	} 
}

int JudgeDir(char * path)
{
	DIR * dir;
	
	dir = opendir(path);
	if(dir == NULL)
	{
		return 0;
	}
	else
	{
		closedir(dir);
		return 1;
	}
}

void ChangePrintColor(char * name)
{

	if(judgeDir == 0)
	{
		printf("%-s", name);
	//printf("debug ===-=========%d ", maxlength);
	}
	else
	{
		printf("\033[35m%-s\033[0m", name);
	}
	printf("\n");
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
	
	if(judgeDir == 0)
	{
		printf("%-s", name);
	//printf("debug ===-=========%d ", maxlength);
	}
	else
	{
		printf("\033[35m%-s\033[0m", name);
	}
	
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
	
	for(i = 0; i < j; i++) //j 是第i个参数的长度
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
		
		else if(parameter[i] |= 'R')
		{
			flag_parameter |= PARAM_R;
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


*/







