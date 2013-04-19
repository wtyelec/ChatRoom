#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <fstream>
#include "client.h"

using namespace std;

#define WRITE_BUF_SIZE 1024
#define READ_BUF_SIZE 1024
#define MAX_CONNECT_NUM 1
#define TEST_SEND_NUM 1000

void *input_msg(void *arg);
void client_socket();
void input_chat_name();
void input_target_name();
void get_ip_local();

char        addrs_buf[15];
char        buf_recv[READ_BUF_SIZE];
char        buf_send[WRITE_BUF_SIZE];
bool        flag_time(false);
int         sClient[MAX_CONNECT_NUM];
int         sleep_sec(4);
void        *tret;
time_t      start,end;
double      dif(2);
pthread_t   tid[2] = {0};
pthread_mutex_t     mutex;
pthread_attr_t      attr;
struct  sockaddr_in ser;
char*        test_name;

int main(int argc, char* argv[])
{
    memset(buf_recv, 0, sizeof(buf_recv));
    memset(buf_send, 0, sizeof(buf_send));
    get_ip_local();
    client_socket();	
    input_chat_name();
    for(int i = 0; i < TEST_SEND_NUM; i++)
    {
        net_packet packet;
        char send_buf[] = "all:abcdefga";
        packet.body = send_buf;
        packet.head.m_chat_type = ALL;
        packet.head.body_size = sizeof(send_buf);
        int write_ret = write(sClient[0], (char*)&packet.head, sizeof(net_packet_head));
        write(sClient[0], packet.body, packet.head.body_size);
        cout << "send_content = " << packet.body << " i = " << i + 1 << endl;
    }

    return 0;
}
void client_socket()
{
    ser.sin_family = AF_INET;
    ser.sin_port = htons(6666);
    inet_pton(AF_INET, addrs_buf, &ser.sin_addr);
    sClient[0] = socket(AF_INET, SOCK_STREAM, 0);	
    connect(sClient[0], (struct sockaddr *)&ser, sizeof(ser));   
}
void input_chat_name()
{
    char name[10];

    cout << "input name" << endl;
    while(cin >> name)
    {
        if(strlen(name) < 10)
        {
            break;
        }
        else 
        {
            cout << "name too long,input again" << endl;
            continue;
        }
    }
    write(sClient[0], name, sizeof(name)); 
}
void get_ip_local() 
{
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct != NULL)
    {
        if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
        {
            if(strcmp(ifAddrStruct->ifa_name, "lo"))
            {
                tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, tmpAddrPtr, addrs_buf, INET_ADDRSTRLEN);
                printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addrs_buf);
            }
        }
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
}
