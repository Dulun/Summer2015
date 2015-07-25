/*************************************************************************
    > File Name: diffork.c
    > Author: Mr.Du
    > Mail: dlxiyoulinux@gmali.com
    > Created Time: 2015年07月24日 星期五 17时33分10秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int globvar = 5;


int main()
{
	pid_t pid;
	int var = 1, i;

	printf("fork is different with vfrok\n");

	pid = vfork();  //pid = fork;

	switch(pid)
	{
		case 0:
				i = 3; 

				while(i -- > 0)
				{
					printf("child process is running \n");
					globvar++;
					sleep(1);
					var ++;
				}

				printf("Child's globvar = %d , var = %d\n ", globvar, var);
				sleep(1);
				exit(0);
				break;

		case -1 :
			perror("Process creation failed\n");
			break;
		default:
			i = 5;
			printf("var = %d\n", var);

			while(i-- > 0)
			{
				printf("Parent process is running, var = %d\n", var);
				globvar++;
				var++;
				sleep(1);
			}

			printf("Parent 's globvar = %d, var = %d \n", globvar , var);
			
	}
	
	//exit(0);
	return 0;
	
}
