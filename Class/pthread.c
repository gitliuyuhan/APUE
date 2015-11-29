/*======================================================
    > File Name: pthread.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年10月21日 星期三 10时44分03秒
 =======================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<pthread.h>

#define MAX_THREAD  3  //线程个数

//初始化锁
static pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned long long   main_counter,counter[MAX_THREAD]={0};

void* thread_worker(void*  arg)
{
//将指针强转为int
    int  thread_num = *(int*)arg;
    printf("thread_id:%lu   counter[%d]\n",pthread_self(),thread_num);
//    printf("%p %d\n",(int*)arg,sizeof((int*)arg));
//释放临时内存
    free((int*)arg);    
//    int   thread_num = (int)arg;
    for(;;)
    {
//加锁
        pthread_mutex_lock(&mutex);
        printf("thread\n");
        counter[thread_num]++;    //本线程的counter 加 1
        main_counter++;
//解锁
//        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc,char* argv[])
{
    int                 i,rtn,ch;
    pthread_t           pthread_id[MAX_THREAD] =  {0};  //存放线程
//保存参数
//    int                 param[3];
    int                 *param;
    for(i=0;i<MAX_THREAD;i++)
    {
//传 &i 
//        pthread_create(&pthread_id[i],NULL,thread_worker,&i);
//传 i 值        
//        pthread_create(&pthread_id[i],NULL,thread_worker,(void*)i);
//数组        
//          param[i] = i;
//          pthread_create(&pthread_id[i],NULL,thread_worker,param+i);
//动态申请内存临时保存参数
        param = (int*)malloc(sizeof(int));
        *param = i; 
        pthread_create(&pthread_id[i],NULL,thread_worker,param);
    }

    do
    {
        unsigned long long   sum = 0;
        for(i=0;i<MAX_THREAD;i++)
        {
            sum += counter[i];
            printf("No.%d: %llu\n",i,counter[i]);
        }
        printf("%llu/%llu\n",main_counter,sum);
//解锁
        pthread_mutex_unlock(&mutex);
        printf("unlock\n");
    }while((ch = getchar())!='q');

//销毁锁
//    pthread_mutex_destroy(&mutex);
    return 0;
}
