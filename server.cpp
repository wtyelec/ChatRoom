#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define RECV_BUF_SIZE 1024

void send_msg(int sid);
void accept_client();
void server_socket();

char    name_data[10];
char    buf_recv[RECV_BUF_SIZE];
int     sListen(0);
int     fdp_max(0);

fd_set              fds;
socklen_t           iLen(0);						
set<int>            socket_s;
map<int,string>     name_buf;

struct timeval      timeout;
struct sockaddr_in  ser;
struct sockaddr_in  cli;		

int main(int argc, char* argv[])
{
    memset(buf_recv,0,sizeof(buf_recv));
    timeout.tv_sec = timeout.tv_usec = 0;
    server_socket();

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(sListen, &fds);
        for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
        {
            FD_SET(*it, &fds);
            fdp_max = fdp_max < *it ? *it:fdp_max;
        }
        fdp_max = fdp_max < sListen ? sListen:fdp_max;
        switch(select(fdp_max +1, &fds, NULL, NULL, &timeout))
        {
            case -1:
                cout << "select error!" << endl;
                break;
            case 0:
                break;
            default:
                if(socket_s.size() > 0)
                {
                    for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
                    {
                        if(FD_ISSET(*it, &fds))
                        {
                            send_msg(*it);
                        }
                    }
                }
                if(FD_ISSET(sListen, &fds))
                {
                    accept_client();
                }
        }
    }

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
void send_msg(int sid)
{
    char name_c[30] = ""; 
    const char* name_m = name_buf[sid].c_str(); 
    int recv_int = recv(sid, buf_recv, sizeof(buf_recv), 0);
    if( recv_int > 0)
    {
        cout << "recv data from " << name_m << ": " << buf_recv << endl;
        if(socket_s.size() > 1)
        {
            strcat(name_c, name_m);
            strcat(name_c, ": ");
            strcat(name_c, buf_recv);

            for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
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
        socket_s.erase(sid);
        cout << sid << "has removed" << endl;
    }
}
// 线程2:监听客户端连接请求
void accept_client()
{
    int tmp = accept(sListen, (struct sockaddr *)&cli, &iLen);
    cout << "accept = " << tmp << endl;
    // 接受client用户名存入name_data
    recv(tmp, name_data, sizeof(name_data),0);
    name_buf.insert(pair<int,string>(tmp, name_data)); 
    socket_s.insert(tmp);
    cout << "port:" << ntohs(cli.sin_port) << " num:" << socket_s.size() << endl;
}
