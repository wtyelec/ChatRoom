#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define RECV_BUF_SIZE 1024

void *thrd_send(void *arg);
void *accept_client(void *arg);
void server_socket();

char        buf_recv[RECV_BUF_SIZE];
int         sListen(0);
int         chat_num(0);
void        *tret;
int         Accepts[10] = {0};    
pthread_t   thread[5] = {0};
pthread_t   tid_apt[5] = {0};
socklen_t   iLen(0);						

map<int,char*>  chat_name;
char            name_data[10][10];

struct sockaddr_in ser,cli;		

int main()
{
    memset(buf_recv,0,sizeof(buf_recv));
    server_socket();

    while(1)
    {
        pthread_create(&tid_apt[chat_num], NULL, accept_client, (void *)chat_num);
        for(int i = 0; i < chat_num; ++i)
        {
            pthread_create(&thread[i], NULL, thrd_send, (void *)i);
        }
        if((pthread_join(tid_apt[chat_num],&tret) == 0))
        {
            continue;
        }
    }
	shutdown(sListen,2);
    return 0;
}
// socket通信
void server_socket()
{
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = INADDR_ANY;
    ser.sin_port = htons(6666);
    sListen = socket(AF_INET,SOCK_STREAM,0);
    bind(sListen,(struct sockaddr*)&ser,sizeof(ser));
    listen(sListen,5);
    iLen = sizeof(cli);
    //等待首个client连接
    Accepts[0] = accept(sListen,(struct sockaddr *)&cli,&iLen);
    printf("port:[%d]\n",ntohs(cli.sin_port));
    recv(Accepts[0],name_data[0],sizeof(name_data),0);
    chat_name[0] = name_data[0];
    chat_num ++;
}
// 线程1:发送消息给其他客户端
void *thrd_send(void *arg)
{
    int     thrd_num = (intptr_t)arg;
    char    name_c[30] = " from "; 
    char*   name_m = chat_name.find(thrd_num)->second; 

    recv(Accepts[thrd_num],buf_recv,sizeof(buf_recv),0);
    cout << "recv data from " << name_m << ":" << buf_recv << endl;
    strcat(name_c,name_m);
    strcat(buf_recv,name_c);

    for(int i = 0; i < chat_num; ++i)
    {
        if(i != thrd_num)
        {
            send(Accepts[i],buf_recv,sizeof(buf_recv),0);
            pthread_cancel(thread[i]);
        }
    }
    pthread_cancel(tid_apt[chat_num]);
    pthread_exit((void *)0);
}
// 线程2:监听客户端连接请求
void *accept_client(void *arg)
{
    Accepts[chat_num] = accept(sListen,(struct sockaddr *)&cli,&iLen);
    printf("port:[%d]\n",ntohs(cli.sin_port));
    // 接受client用户名存入name_data
    recv(Accepts[chat_num],name_data[chat_num],sizeof(name_data),0);
    chat_name[chat_num] = name_data[chat_num];

    for(int i = 0; i < chat_num; ++i)
    {
        pthread_cancel(thread[i]);
    }
    chat_num ++;
}
