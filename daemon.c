/*************************************************************************
    > File Name: daemon.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月25日 星期六 09时43分30秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<sys/param.h>
#include<syslog.h>

int init_daemon(void)
{
	int pid;
	int i;

	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	pid = fork();
	if(pid > 0)
	{
		exit(0);
	}
	else if(pid < 0)
	{
		return -1;
	}

	setsid();

	pid = fork();
	if(pid > 0)
	{
		exit(0);	
	}
	else if(pid < 0)
	{
		return -1;
	}

	for(i = 0 ; i < NOFILE; close(i++))
		;

	chdir("/");

	umask(0);

	signal(SIGCHLD, SIG_IGN);
	return 0;
}

int main()
{
	time_t now;
	init_daemon();
	syslog(LOG_USER | LOG_INFO, "测试守护进程\n");
	while(1)
	{
		sleep(2);
		time(&now);

		syslog(LOG_USER | LOG_INFO, "系统时间：\t%s\t\t\n", ctime(&now));
	}
}
