/*======================================================
    > File Name: fork_kill.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年10月14日 星期三 14时03分32秒
 =======================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include<ctype.h>

/*允许建立子进程个数最大值*/
#define MAX_CHILD_NUMBER     10

/*子进程睡眠时间*/
#define SLEEP_INTERVAL       4

void do_something();

int proc_number = 0;  //子进程的自编号，从0开始

int main(int argc,char* argv[])
{
    //子进程个数
    int            child_proc_number = MAX_CHILD_NUMBER;
    int            i,ch;
    pid_t          child_pid;
    pid_t          pid[10] = {0}; //存放每个子进程的id

    if(argc > 1)
    {
        child_proc_number = atoi(argv[1]);
        child_proc_number = (child_proc_number > 10) ? 10 : child_proc_number;
    }

    signal(SIGCHLD,SIG_IGN);

    for(i = 0;i<child_proc_number;i++)
    {
        child_pid = fork();
        switch(child_pid)
        {
            case 0:
                proc_number = i;
                do_something();
                break;
            case -1:
                printf("No.%d childProcess created failed!\n",i);
                break;
            default:
                pid[i] = child_pid;
        }
    }

    while((ch = getchar()) != 'q')
    {
        if(ch != 13)
        {
            getchar(); //处理回车
        }
        if(isdigit(ch))
        {
            if(ch >= '0' && (ch-'0') < MAX_CHILD_NUMBER)
            {
                if(kill(pid[ch-'0'],SIGTERM) == 0)
                {
                    printf("No.%d is killed\n",ch-'0');
                    waitpid(pid[ch-'0'],NULL,WNOHANG);
                }
            }
            else
            {
                printf("have no this process!\n");
            }
        }
        else
        {
            printf("No. error\n");
        }
    }
    kill(0,SIGTERM);
    return 0;
}

void do_something()
{
    for(;;)
    {
        printf("This is process No.%d and its pid is %d\n",proc_number,getpid());
        sleep(SLEEP_INTERVAL);
    }
}
