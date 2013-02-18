#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <stdio.h>
#include <time.h>

extern sock_info_t          g_sock_info;
extern map<string,int>      g_name_sock;
extern map<int,string>      g_sock_name;
extern fd_set               g_all_set;
extern int                  g_max_fd;

class log
{
public:
	inline static void log_current_time()
	{
		time_t rawtime;
		time(&rawtime);
		cout << "time: " << ctime(&rawtime);
	}
};

#endif
