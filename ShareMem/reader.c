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

#define SHM_SIZE  1024

int main()
{
    int       shmid;
    char      *shmaddr;
    key_t     shmkey;

    if((shmkey = ftok(".",'m'))==-1)
    {
        perror("ftok error!\n");
        return -1;
    }
    if((shmid = shmget(shmkey,SHM_SIZE,IPC_CREAT | 0666))==-1)
    {
        perror("shmget call failed.\n");
        return -1;
    }
    if((shmaddr = shmat(shmid,(char*)0,0)) == (char*)-1)
    {
        perror("attach shared memory error!\n");
        return -1;
    }
    int  i=5;
    //读数据5次
    while(i)
    {
        printf("reader: ");
        printf("%s\n",shmaddr);
        sleep(10);
        i--;
    }
    //与共享内存断开连接
    if(shmdt(shmaddr)==0)
    {
        printf("quit\n");
    }
    return 0;
}
