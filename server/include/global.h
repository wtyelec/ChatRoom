#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <time.h>

extern sock_info_t          g_sock_info;
extern map<string,int16_t>	g_name_sock;
extern map<int16_t,string>	g_sock_name;
extern fd_set               g_all_set;
extern int16_t              g_max_fd;

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

enum chat_type
{
    NAME = 1,
    ALL = 2,
    PRIVATE = 3 
};

struct net_packet_head
{
    int       body_size;
    chat_type m_chat_type;
};

struct net_packet
{
    net_packet_head head;
    const char*     body;
};

#endif
