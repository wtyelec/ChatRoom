#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
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
void    *tret;

socklen_t           iLen(0);						
pthread_t           tid_apt(0);
pthread_mutex_t     mutex;
pthread_attr_t      attr;
set<int>            socket_v;
map<int,pthread_t>  tid_m;
map<int,string>     name_buf;
char                name_data[10];

struct sockaddr_in ser,cli;		

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex,NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  
    memset(buf_recv,0,sizeof(buf_recv));
    server_socket();

    while(1)
    {
        int i(0);
        pthread_create(&tid_apt, NULL, accept_client, (void *)socket_v.size());
        for(set<int>::iterator it = socket_v.begin(); it != socket_v.end(); it++)
        {
            tid_m.insert(pair<int,pthread_t>(*it, i++)); 
            pthread_create(&tid_m[*it], &attr, send_msg, (void *)*it);
        }
        if((pthread_join(tid_apt, &tret) == 0))
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
    sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (struct sockaddr*)&ser, sizeof(ser));
    listen(sListen, 5);
    iLen = sizeof(cli);
}
// 线程1:发送消息给其他客户端
void *send_msg(void *arg)
{
    int sid = (intptr_t)arg;
    char name_c[30] = ""; 
    const char* name_m = name_buf[sid].c_str(); 
    int recv_int = recv(sid, buf_recv, sizeof(buf_recv), 0);
    pthread_mutex_lock(&mutex);
    if( recv_int > 0)
    {
        cout << "recv data from " << name_m << ": " << buf_recv << endl;
        if(socket_v.size() > 1)
        {
            strcat(name_c, name_m);
            strcat(name_c, ": ");
            strcat(name_c, buf_recv);

            for(set<int>::iterator it = socket_v.begin(); it != socket_v.end(); it++)
            {
                if(*it != sid)
                {
                    send(*it, name_c, sizeof(name_c),0);
                }
            }
        }
    }
    else
    {
        socket_v.erase(sid);
        cout << sid << "has removed" << endl;
    }
    //pthread_cancel(tid_apt);
    pthread_mutex_unlock(&mutex);
    pthread_create(&tid_m[sid], &attr, send_msg, (void *)sid);
    pthread_exit((void *)0);
}
// 线程2:监听客户端连接请求
void *accept_client(void *arg)
{
    int tmp = accept(sListen, (struct sockaddr *)&cli, &iLen);
    pthread_mutex_lock(&mutex);
    // 接受client用户名存入name_data
    recv(tmp, name_data, sizeof(name_data),0);
    name_buf.insert(pair<int,string>(tmp, name_data)); 
    socket_v.insert(tmp);
    cout << "port:" << ntohs(cli.sin_port) << " num:" << socket_v.size() << endl;
    //tid_v.push_back(0);
    pthread_mutex_unlock(&mutex);
    pthread_exit((void *)0);
}
