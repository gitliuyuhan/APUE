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

#define BUFSIZE   100
#define MAX       2

//异步读结构体
int aio_read_file(struct aiocb *cbp,int fd,int size)
{
    int               ret;
    bzero(cbp,sizeof(struct aiocb));

    cbp->aio_buf = (volatile void*)malloc(size+1);
    cbp->aio_nbytes = size;
    cbp->aio_offset = 0;
    cbp->aio_fildes = fd;
    cbp->aio_lio_opcode = LIO_READ;
}

int main()
{
    struct aiocb    cbp1,cbp2;
    int             fd1,fd2,ret;
    int             i = 0;
    //异步请求列表
    struct aiocb*   io_list[2];

    fd1 = open("test.txt",O_RDONLY);
    if(fd1 < 0)
    {
        perror("open error\n");
    }
    aio_read_file(&cbp1,fd1,BUFSIZE);

    fd2 = open("test.txt",O_RDONLY);
    if(fd2 < 0)
    {
        perror("open error\n");
    }
    aio_read_file(&cbp2,fd2,BUFSIZE*4);

    io_list[0] = &cbp1;
    io_list[1] = &cbp2;
    
    lio_listio(LIO_WAIT,(struct aiocb* const*)io_list,MAX,NULL);  
    printf("read1:%s\n",(char*)cbp1.aio_buf);
    printf("read2:%s\n",(char*)cbp2.aio_buf);

    close(fd1);
    close(fd2);
    return 0;
}
