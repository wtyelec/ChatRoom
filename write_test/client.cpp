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
#include <fcntl.h>
#include <fstream>
#include "client.h"

using namespace std;

#define READ_BUF_SIZE 1024
#define TEST_SEND_NUM 1000
#define MAX_CONNECT_NUM 1

void *recv_ser(void *arg);
void *input_msg(void *arg);
void client_socket();
void input_chat_name();
void get_ip_local();
void get_ip_config();
void heartbeat_cli(int alarm_sec, int max_probes);
void sig_alrm(int signo);
int packet_write(int write_fd_, string& body_, packet_type type_);
//int packet_write(int write_fd_, const char* body_, int16_t body_size_);
int packet_read(int read_fd_, char* &buf_send);

char        addrs_buf[15];
char        buf_recv[READ_BUF_SIZE];
bool        flag_time(false);
int        	serv_fd(0); 
int         sleep_sec(4);
void        *tret;
time_t      start,end;
double      dif(2);
pthread_t   tid[2] = {0};
pthread_mutex_t     mutex;
pthread_attr_t      attr;
struct  sockaddr_in ser;
string name;
char name_feedback[50];

int alarm_sec(0);
int max_probes(0);
int cur_probes(0);
int recv_count(0);

int main(int argc, char* argv[])
{
	get_ip_local();
	client_socket();	
    input_chat_name();
    string send_buf = "all:fasdf";
    for(int i = 0; i < TEST_SEND_NUM; i++)
    {
        packet_write(serv_fd, send_buf, ALL);
        cout << "count: " << i << endl;
    }
}

void heartbeat_cli(int alarm_sec_, int max_probes_)
{
	alarm_sec = alarm_sec_;
	max_probes = max_probes_;
	cur_probes = 0;
	signal(SIGALRM, sig_alrm);
	alarm(alarm_sec);
}	

void sig_alrm(int signo)
{
	//packet_write(serv_fd, "heart:beat", sizeof("heart:beat"));	
	alarm(alarm_sec);
	return;
}

void client_socket()
{
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6666);
	inet_pton(AF_INET, addrs_buf, &ser.sin_addr);
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);	
	connect(serv_fd, (struct sockaddr *)&ser, sizeof(ser));   
    /*
	do{
		memset(name_feedback, 0, sizeof(name_feedback));
		input_chat_name();
		write(serv_fd, name.data(), sizeof(name.data())); 
		read(serv_fd, name_feedback, sizeof(name_feedback));
		cout << name_feedback << endl;
	}while(name_feedback[0] !=  'p');
    */
}

void input_chat_name()
{
	cout << "input name" << endl;
	while(cin >> name)
	{
		if(name.size() < 10)
		{
			break;
		}
		else 
		{
			cout << "name too long,input again" << endl;
			continue;
		}
	}
    //write(serv_fd, name.data(), name.size());
    packet_write(serv_fd, name, NAME);
}

int packet_read(int read_fd_, char* &buf_read)
{
    // read net_packet_head
	char buf_head[sizeof(net_packet_head)];
	memset(buf_head, 0, sizeof(buf_head));
	int read_head_ret = read(serv_fd, buf_head, sizeof(buf_head));
    int read_body_ret(0); 
	if(read_head_ret > 0)
	{
        net_packet packet;
        packet.head = *(net_packet_head*)buf_head;
        // read net_packet_body
        int read_body_ret = read(serv_fd, buf_read, packet.head.body_size);

        return read_body_ret;
    }
    if(read_head_ret <= 0 || read_body_ret <=0)
    {
        cout << "disconnect with server" << endl;
        close(serv_fd);
        exit(0);
    }

    return read_head_ret;
}

int packet_write(int write_fd_, string& body_, packet_type type_)
{
    net_packet write_packet;
    write_packet.head.body_size = body_.size() + 1;
    write_packet.head.m_packet_type = type_;
    write(write_fd_, (char*)&write_packet.head, sizeof(net_packet_head));
    int write_ret = write(write_fd_, body_.data(), body_.size() + 1);

    return write_ret;
}

void *recv_ser(void *arg)
{
    char* buf_read;
    packet_read(serv_fd, buf_read);
    cout << buf_read << endl;
    pthread_create(&tid[0], &attr, recv_ser, NULL);
	pthread_exit((void *)0);
}

void *input_msg(void *arg)
{
    string buf_send;
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
		dif = difftime(end, start);
		if(dif > -2 && dif < 2)						// forbid inputting frequently 
		{
			printf("input too fast! wait\n");
			break;
		}
		else if(buf_send.size() > 20)				// forbid inputting too long message
		{
			printf("message too long! in 20\n");
			continue;
		}
		else
		{
            /*
            net_packet_body packet_body;
            strcpy(packet_body.receiver, "all");
            //packet_body.receiver= "all";
            //unsigned char[] test_byte = System.Text.Encoding.Default.GetBytes("all");
            //packet_body.receiver = test_byte; 
            //packet_body.message = buf_send.data();
            packet_body.recv_id = 3;
            cout << "recv_if:" << packet_body.recv_id << endl;
            cout << "receiver:" << packet_body.receiver << endl;
            cout << "message:" << packet_body.message << endl;
            char write_buf[sizeof("all") + buf_send.size() + 1];
            memset(write_buf, 0, sizeof(write_buf));
            memcpy(write_buf, &packet_body, sizeof(packet_body));
            */
            int write_ret = packet_write(serv_fd, buf_send, ALL);
            //cout << "write_ret = " << write_ret << endl;
			break;
		}
	}
	pthread_create(&tid[1], &attr, input_msg, NULL);
	pthread_exit((void *)1);
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
			if(strcmp(ifAddrStruct->ifa_name, "lo0"))
			{
				if(strcmp(ifAddrStruct->ifa_name, "en0"))
				{	
					tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
					inet_ntop(AF_INET, tmpAddrPtr, addrs_buf, INET_ADDRSTRLEN);
					printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addrs_buf);
				}
			}
		}
		ifAddrStruct=ifAddrStruct->ifa_next;
	}
}

void get_ip_config()
{
	ifstream fin("config.txt");  
	fin.getline(addrs_buf, 15);
	cout << "Read from file: " << addrs_buf << endl; 
}
