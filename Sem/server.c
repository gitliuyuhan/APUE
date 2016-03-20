/*======================================================
    > File Name: server.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月20日 星期日 17时22分38秒
 =======================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<linux/sem.h>

#define MAX_RESOURCE  6

int main()
{
    key_t     key;
    int       semid;
    //第0个信号资源，每次取1个，阻塞方式
    struct sembuf  sbuf0 = {0,-1,IPC_NOWAIT};
    //负值表示取资源，正值表示放资源
    struct sembuf  sbuf1 = {0,-1,IPC_NOWAIT};

    struct sembuf  sbuf[2];
    union  semun   semopts;

    //生成关键字
    if((key = ftok(".",'s'))==-1)
    {
        perror("ftok error!\n");
        return 1;
    }
    //创建信号集，共有1个信号资源
    if((semid = semget(key,1,IPC_CREAT|0666))==-1)
    {
        perror("semget error!\n");
        return 1;
    }
    //设置信号量值
    semopts.val = MAX_RESOURCE;
    if(semctl(semid,0,SETVAL,semopts)==-1)
    {
        perror("semctl error!\n");
        return 1;
    }

    //可以一次性执行多个操作
    sbuf[0] = sbuf0;
    sbuf[1] = sbuf1;
    while(1)
    {
        sleep(3);
        //第3个参数是操作个数
        if(semop(semid,&sbuf,2)==-1)
        {
            perror("semop error!\n");
            return 1;
        }
    }
    return 0;
}

