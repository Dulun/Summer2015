//linuxc#cat myshell.c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_STR 30
#define MAX_PARA 5
int main()
{
        pid_t p1,p2;
        char buf[MAX_STR],bufile[MAX_STR], *ps_argv[MAX_PARA],*argv1[MAX_PARA],*argv2[MAX_PARA];
        int i,n,fd[2],save_fd;

        while(1)
        {
                write(STDOUT_FILENO,"myshell> ",9);
                memset(buf,0,MAX_STR);
                n = read(STDIN_FILENO, buf, MAX_STR-1);
                buf[strlen(buf)-1]='\0';
                if (n < 0) {
                        perror("read error");
                        _exit(1);
                }
                else if (n == 1) continue;
                if ( !(strncmp(buf, "quit", 4))) {
                        write(STDOUT_FILENO,"bye-bye!\n",9);
                        break;
                }
                p1 = fork();
                if (p1 < 0) {
                        perror(" fork error");
                        _exit(1);
                }
                if (p1 == 0) {
                        ps_argv[0] = strtok(buf, "|");
                        for(i=1; (ps_argv[i]=strtok(NULL,"|")); i++);

                        if (pipe(fd)<0)
                        {
                                perror("pipe");
                                _exit(1);
                        }
                        if ( (p2=fork()) <0)
                        {
                                perror("fork 2");
                                _exit(1);
                        }
                        if (p2 > 0) //parent

                        {
                                close(fd[0]);
                                save_fd = dup(STDOUT_FILENO);

                                if(strchr(ps_argv[0],(int)'<'))
                                {
                                        argv1[0] = strtok(ps_argv[0], "<");
                                        for(i=1; (argv1[i]=strtok(NULL,"<")); i++);
                                        int temp=open(argv1[1],O_RDONLY);
                                        if (temp < 0)
                                                perror("open argv1[1]");
                                        read(temp,bufile,MAX_STR);
                                        argv1[1] = strtok(bufile," ");
                                        for(i=2; (argv1[i]=strtok(NULL," "));i++);
                                        argv1[i-1]=NULL;
                                } else {
                                        argv1[0] = strtok(ps_argv[0]," ");
                                        for(i=1; (argv1[i]=strtok(NULL," ")); i++);
                                }

                                //for(i=0;i<=3;i++) { write(1,argv1[i],sizeof(argv1[i])); }

                                dup2(fd[1],STDOUT_FILENO);
                                execvp(argv1[0],argv1);
                                perror("unaviable command");
                                dup2(save_fd,STDOUT_FILENO);
                        } else { //child

                                char buff[1400]={0};
                                int nn;

                                close(fd[1]);
                                nn=read(fd[0],buff,1400);
                                if(ps_argv[1])
                                {
                                        if(strchr(ps_argv[1],(int)'>'))
                                        {
                                                argv2[0] = strtok(ps_argv[1],">");
                                                for(i=1; (argv2[i]=strtok(NULL,">")); i++);
                                                int temp=open(argv2[1],O_WRONLY);
                                                if(temp < 0)
                                                        perror("open argv2[1]");
                                                dup2(temp, STDOUT_FILENO);
                                                ps_argv[1]=argv2[0];
                                        }

                                        printf("test:%s\n",ps_argv[1]);
                                        argv2[0] = strtok(ps_argv[1], " ");
                                        for(i=1; (argv2[i]=strtok(NULL, " "));i++);
                                        argv2[i]="fd[0]";
                                        argv2[i+1]=NULL;
                                        write(1,argv2[0],sizeof(argv2[0]));
                                        write(1,"|",1);
                                        write(1,argv2[1],sizeof(argv2[1]));
                                        write(1,"|",1);
                                        write(1,argv2[2],sizeof(argv2[2]));
                                        write(1,"|",1);
                                        write(1,argv2[3],sizeof(argv2[3]));
                                        write(1,"|",1);

                                        printf("\n");
                                        execvp(argv2[0],argv2);
                                } else
                                        write(STDOUT_FILENO,buff,nn);
                                //write(1,"argv2\n",6);

                        }
                        break;
                }
                sleep(1);
        }
        return 0;
}
