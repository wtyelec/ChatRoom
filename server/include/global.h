#ifndef SERVER_H
#define SERVER_H

#include <iostream>
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

#endif
