#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <time.h>
#include <event.h>

extern sock_info_t      g_sock_info;
extern map<string,int>	g_name_sock;
extern map<int,string>	g_sock_name;
extern fd_set           g_all_set;
extern int              g_max_fd;
//extern event            g_serv_ev;

class log
{
	public:
		inline static void log_current_time()
		{
			time_t ticks;
			time(&ticks);
			cout << "time: " << ctime(&ticks);
		}
};

enum packet_type 
{
    NAME = 1,
    HEART,
    ALL,
    PRIVATE,
    EXCEPTION
};

struct net_packet_head
{
    int     body_size;
    packet_type m_packet_type;
};

struct net_packet_body
{
    int     recv_id;
    char        receiver[4];
    char        message[20];
};

struct net_packet
{
    net_packet_head head;
    net_packet_body body;
};

#endif
