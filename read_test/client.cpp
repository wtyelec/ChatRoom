#include <iostream>
#include <string>
#include <stdlib.h>
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

#define SEND_BUF_SIZE 4096
#define RECV_BUF_SIZE 4096 
#define MAX_CONNECT_NUM 1

void recv_ser();
void client_socket();
void input_chat_name();
void input_target_name();
void get_ip_local();
void get_ip_config();

char        addrs_buf[15];
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
int         recv_count(0);

int main(int argc, char* argv[])
{
    get_ip_local();
    client_socket();	
    input_chat_name();
    while(1)
    {
        recv_ser();
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
    send(sClient[0], name, sizeof(name),0); 
}

void recv_ser()
{
    // read net_packet_head
	char buf_head[sizeof(net_packet_head)];
	memset(buf_head, 0, sizeof(buf_head));
	int read_head_ret = read(sClient[0], buf_head, sizeof(buf_head));
    int read_body_ret(0);
	if(read_head_ret > 0)
	{
        net_packet packet;
        packet.head = *(net_packet_head*)buf_head;
        // read net_packet_body
        char buf_body[packet.head.body_size];
        memset(buf_body, 0, sizeof(buf_body));
        read_body_ret = read(sClient[0], buf_body, packet.head.body_size);
        if(read_body_ret > 0)
        {
            if(buf_body[0] != '\0')
            {
                cout << buf_body << ". recv_count = " << ++recv_count << endl;
            }
            buf_body[0] = '\0'; 
        }
    }

    if(read_head_ret <= 0 || read_body_ret <=0)
    {
        cout << "disconnect with server" << endl;
        close(sClient[0]);
        exit(0);
    }
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
