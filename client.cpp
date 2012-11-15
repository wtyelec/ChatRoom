#include <iostream>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 1024

void *recv_ser(void *arg);
void *input_msg(void *arg);
void client_socket();
void input_chat_num();
void input_chat_name();
void read_ip();

int         sClient;
char        buf_recv[1024];
char        buf_send[1024];
void        *tret;
time_t      start,end;
double      dif(2);
bool        flag_time(false);
char        ip[15];   
pthread_t   thread[2] = {0};

struct  sockaddr_in ser;

int main()
{
    memset(buf_recv,0,sizeof(buf_recv));
    memset(buf_send,0,sizeof(buf_send));
    read_ip();
    client_socket();	 
    //recv(sClient,buf_recv,sizeof(buf_recv),0);   
    //input_chat_num();
    input_chat_name();
    
    while(1)
    {
        pthread_create(&thread[0], NULL, recv_ser, NULL);
        pthread_create(&thread[1], NULL, input_msg, NULL);
        if( (pthread_join(thread[0],&tret) == 0))
        {
            continue;
        }
    }

    shutdown(sClient,2);
    return 0;
}
// socket通信
void client_socket()
{
    ser.sin_family = AF_INET;
    ser.sin_port = htons(6666);
    ser.sin_addr.s_addr = inet_addr(ip);//"192.168.86.137");
    sClient = socket(AF_INET,SOCK_STREAM,0);	
    connect(sClient,(struct sockaddr *)&ser,sizeof(ser));
}
// 输入聊天人数,发送到服务器
void input_chat_num()
{
    int chat_num;

    if(strcmp(buf_recv,"chat number") == 0)
    {
        cout << "input chat number" << endl;
        while(chat_num <= 0)
        {
            cin >> chat_num;
        }
        char num[10];
        sprintf(num,"%d",chat_num);
        send(sClient,num,sizeof(num),0);
    }
}
// 输入client用户名,发送到服务器
void input_chat_name()
{
    char name[10];

    cout << "input name" << endl;
    cin >> name;
    send(sClient,name,sizeof(name),0); 
}
// 线程1:接受服务器发来消息
void *recv_ser(void *arg)
{
    recv(sClient,buf_recv,sizeof(buf_recv),0);   
    if(buf_recv[0] != '\0')
        printf("recv data:%s\n",buf_recv);
    buf_recv[0] = '\0'; 
    pthread_cancel(thread[1]);
    pthread_exit((void *)0);
}
// 线程2:输入聊天信息
void *input_msg(void *arg)
{
    while(cin >> buf_send)
    {
        if(flag_time)
        {
            time(&start);
            flag_time = false;
        }
        else
        {
            time(&end);
            flag_time = true;
        }
        dif = difftime(end,start);
        if(dif > -2 && dif < 2)                 // 防止频繁输入
        {
            printf("input too fast! wait\n");
            break;
        }
        else if(strlen(buf_send) >20)           // 防止过长输入
        {
            printf("message too long! in 20\n");
            continue;
        }
        else
        {
            send(sClient,buf_send,sizeof(buf_send),0);
            break;
        }
    }
    pthread_cancel(thread[0]);
    pthread_exit((void *)1);
}

void read_ip()
{
    ifstream fin("config.txt");  
    fin.getline(ip,15);
    cout << "Read from file: " << ip << endl; 
}
