/*======================================================
    > File Name: aio_read.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月14日 星期一 08时39分08秒
 =======================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<aio.h>
#include<stdlib.h>
#include<strings.h>
#include<signal.h>

#define BUFSIZE   256

//回调函数
void aio_handler(sigval_t sigval)
{
    struct aiocb  *cbp;
    int           ret;

    printf("异步操作完成，收到通知\n");
    //获取aiocb 结构体的信息
    cbp = (struct aiocb*)sigval.sival_ptr;

    if(aio_error(cbp) == 0)
    {
        ret = aio_return(cbp);
        printf("读请求返回值：%d\n",ret);
    }
    while(1)
    {
        printf("正在执行回调函数。。。\n");
        sleep(1);
    }
}

int main()
{
    struct aiocb    cbp;
    int             fd,ret;
    int             i = 0;

    fd = open("test.txt",O_RDONLY);

    if(fd < 0)
    {
        perror("open error\n");
    }

    //填充struct aiocb 结构体 
    bzero(&cbp,sizeof(cbp));
    //指定缓冲区
    cbp.aio_buf = (volatile void*)malloc(BUFSIZE+1);
    //请求读取的字节数
    cbp.aio_nbytes = BUFSIZE;
    //文件偏移
    cbp.aio_offset = 0;
    //读取的文件描述符
    cbp.aio_fildes = fd;
    //发起读请求
    
    //设置异步通知方式
    //用线程回调
    cbp.aio_sigevent.sigev_notify = SIGEV_THREAD;
    //设置回调函数
    cbp.aio_sigevent.sigev_notify_function  = aio_handler;
    //传入aiocb 结构体
    cbp.aio_sigevent.sigev_value.sival_ptr = &cbp;
    //设置属性为默认
    cbp.aio_sigevent.sigev_notify_attributes = NULL;

    //发起异步读请求
    ret = aio_read(&cbp);
    if(ret < 0)
    {
        perror("aio_read error\n");
        exit(1);
    }
    //调用进程继续执行
    while(1)
    {
        printf("主线程继续执行。。。\n");
        sleep(1);
    }
    close(fd);
    return 0;
}
