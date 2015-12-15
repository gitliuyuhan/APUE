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
#include<string.h>

int main()
{
    struct aiocb    cbp;
    int             fd,ret;
    int             i = 0;

    //以追加的方式打开
    fd = open("test.txt",O_WRONLY);

    if(fd < 0)
    {
        perror("open error\n");
    }

    //填充struct aiocb 结构体 
    bzero(&cbp,sizeof(cbp));
    //指定缓冲区
    cbp.aio_buf = "hello,world";
    //请求写入的字节数
    cbp.aio_nbytes = strlen((const char*)cbp.aio_buf);
    //文件偏移
    cbp.aio_offset = 20;
    //文件描述符
    cbp.aio_fildes = fd;
    //发起写请求
    ret = aio_write(&cbp);
    if(ret < 0)
    {
        perror("aio_read error\n");
        exit(1);
    }

    //查看异步写的状态，直到写请求完成
    for(i = 1;aio_error(&cbp) == EINPROGRESS;i++)
    {
        printf("No.%3d\n",i);
    }
    ret = aio_return(&cbp);
    printf("return %d\n",ret);
    close(fd);
    return 0;
}
