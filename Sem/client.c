/*======================================================
    > File Name: client.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月20日 星期日 16时55分38秒
 =======================================================*/

#include<sys/types.h>
#include<linux/sem.h>
#include<stdio.h>

int main()
{
    key_t           key;
    int             semid,semval;
    union semun     semopts;

    if((key = ftok(".",'s')) == -1)
    {
        perror("ftok error!\n");
        return 1;
    }
    //创建信号集，如果存在返回信号集id
    if((semid = semget(key,1,IPC_CREAT | 0666)) == -1)
    {
        perror("semget error!\n");
        return 1;
    }
    while(1)
    {
        //GETVAL表示返回的是第0个信号的剩余信号量值
        if((semval = semctl(semid,0,GETVAL,0))==-1)
        {
            perror("semctl error!\n");
            return 1;
        }
        if(semval > 0)
        {
            printf("Still have %d resources can be used\n",semval);
        }
        else
        {
            printf("No more resources can be used!\n");
            break;
        }
        sleep(3);
    }
    return 0;
}
