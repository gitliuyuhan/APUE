/*======================================================
    > File Name: opmsg.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年11月27日 星期五 20时37分05秒
 =======================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<time.h>

#define BUF_SIZE    256
#define PROJ_ID     32
#define PATH_NAME   "."    //当前目录

//输出消息队列的属性
void getmsgattr(int msgid,struct msqid_ds  msq_info);

int main()
{
    //自定义消息缓冲区
    struct mymsgbuf
    {
        long      msgtype;
        char      msgstring[BUF_SIZE];
    }msgbuffer;

    int             msgid;         //消息队列标识符
    int             msglen;        //消息长度
    key_t           msgkey;        //消息队列关键字
    struct msqid_ds msg_attr;      //属性

    //获取键值
    if((msgkey = ftok(PATH_NAME,PROJ_ID)) == -1 )
    {
        perror("ftok error!\n");
        exit(1);
    }

    //获取消息队列标识符
    if((msgid = msgget(msgkey,IPC_CREAT|0660)) == -1)
    {
        perror("msgget error!\n");
        exit(1);
    }

    //输出消息队列的属性
    getmsgattr(msgid,msg_attr);

    //发送一条消息到消息队列
    msgbuffer.msgtype = 2;
    strcpy(msgbuffer.msgstring,"hello,world");
    msglen = sizeof(struct mymsgbuf);

    if(msgsnd(msgid,&msgbuffer,msglen,0) == -1)
    {
        perror("msgsnd error!\n");
        exit(1);
    }

    //输出消息队列属性
    getmsgattr(msgid,msg_attr);

    //设置消息队列属性
    msg_attr.msg_perm.uid = 3;
    msg_attr.msg_perm.gid = 2;

    if(msgctl(msgid,IPC_SET,&msg_attr) == -1)
    {
        perror("msg set error!\n");
        exit(1);
    }

    //输出修改后的属性
    getmsgattr(msgid,msg_attr);

    //从内核中删除消息队列
    if(msgctl(msgid,IPC_RMID,NULL) == -1)
    {
        perror("delete msg error!\n");
        exit(1);
    }

    //输出删除后的属性
    getmsgattr(msgid,msg_attr);

    exit(0);
}

void getmsgattr(int msgid,struct msqid_ds msg_info)
{
    if(msgctl(msgid,IPC_STAT,&msg_info) == -1)
    {
        perror("msgctl error!\n");
        return;
    }

    printf("**** information of message quene %d ****\n",msgid);
    printf("向消息队列发送最后一条消息的时间：%s\n",ctime((&msg_info.msg_stime)));
    printf("从消息队列接收最后一条消息的时间：%s\n",ctime(&(msg_info.msg_rtime)));
    printf("最后一次变更消息队列的时间：%s\n",ctime(&(msg_info.msg_ctime)));
    printf("消息队列中所有消息占的字节数：%ld\n",msg_info.msg_cbytes);
    printf("消息队列中消息的数目：%ld\n",msg_info.msg_qnum);
    printf("消息队列的最大字节数：%ld\n",msg_info.msg_qbytes);
    printf("发送最后一条消息的进程ID：%d\n",msg_info.msg_lspid);
    printf("读取最后一条消息的进程ID：%d\n",msg_info.msg_lrpid);
    printf("消息队列的用户ID：%d\n",msg_info.msg_perm.uid);
    printf("消息队列的组ID：%d\n",msg_info.msg_perm.gid);

    printf("********** information end! ************\n");
}
