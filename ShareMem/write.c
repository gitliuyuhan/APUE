/*======================================================
    > File Name: write.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月20日 星期日 19时41分56秒
 =======================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include<string.h>

#define SHM_SIZE  1024

int main()
{
    int       shmid;
    char      *shmaddr;
    char      write_str[SHM_SIZE];
    key_t     shmkey;

    //获取关键字
    if((shmkey = ftok(".",'m'))==-1)
    {
        perror("ftok error!\n");
        return -1;
    }
    //创建或获取共享内存
    if((shmid = shmget(shmkey,SHM_SIZE,IPC_CREAT | 0666))==-1)
    {
        perror("shmget call failed.\n");
        return -1;
    }
    //将共享内存与进程连接
    if((shmaddr = shmat(shmid,(char*)0,0)) == (char*)-1)
    {
        perror("attach shared memory error!\n");
        return -1;
    }
    //写数据3次
    int i=3;
    while(i)
    {
        printf("write: ");
        scanf("%s",write_str);
        int len = strlen(write_str);
        write_str[len] = '\0';
        strcpy(shmaddr,write_str);
        sleep(10);
        i--;
    }
    //与共享内存断开连接
    if(shmdt(shmaddr) == 0)
    {
        printf("quit\n");
    }
    return 0;
}
