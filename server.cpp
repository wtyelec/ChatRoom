#include <iostream>
#include <string>
#include <vector>
#include <map>
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

void *send_msg(void *arg);
void *accept_client(void *arg);
void listen_client();
void server_socket();

char    buf_recv[RECV_BUF_SIZE];
int     sListen(0);
int     chat_num(0);
void    *tret;

socklen_t           iLen(0);						
pthread_t           tid_apt(0);
vector<int>         socket_v;
vector<pthread_t>   tid_v;
map<int,string>     name_buf;
char                name_data[10];

struct sockaddr_in ser,cli;		

int main()
{
    memset(buf_recv,0,sizeof(buf_recv));
    server_socket();

    while(1)
    {
        chat_num = socket_v.size();
        pthread_create(&tid_apt, NULL, accept_client, (void *)chat_num);
        for(int i = 0; i < socket_v.size(); ++i)
        {
            pthread_create(&tid_v[i], NULL, send_msg, (void *)socket_v[i]);
        }
        if((pthread_join(tid_apt,&tret) == 0))
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
}
// 线程1:发送消息给其他客户端
void *send_msg(void *arg)
{
    int     sid = (intptr_t)arg;
    char    name_c[30] = ""; 
    const char* name_m = name_buf[sid].c_str(); 
    int recv_int = recv(sid,buf_recv,sizeof(buf_recv),0);
    if( recv_int > 0)
    {
        cout << "recv data from " << name_m << ": " << buf_recv << endl;
        strcat(name_c,name_m);
        strcat(name_c,": ");
        strcat(name_c, buf_recv);

        for(int i = 0; i < socket_v.size(); ++i)
        {
            if(socket_v[i] != sid)
            {
                send(socket_v[i], name_c, sizeof(name_c),0);
            }
        }
        pthread_cancel(tid_apt);
    }
    else
    {
        //socket_v.erase(socket_v.begin() + thrd_num);
        cout << sid << "has removed" << endl;
    }
    pthread_exit((void *)0);
}
// 线程2:监听客户端连接请求
void *accept_client(void *arg)
{
    socket_v.push_back(accept(sListen,(struct sockaddr *)&cli,&iLen));
    printf("port:[%d] num = %d\n",ntohs(cli.sin_port),chat_num+1);
    // 接受client用户名存入name_data
    recv(socket_v[socket_v.size()-1],name_data,sizeof(name_data),0);
    name_buf.insert(pair<int,string>(socket_v[socket_v.size()-1], name_data)); 
    tid_v.push_back(0);
}
