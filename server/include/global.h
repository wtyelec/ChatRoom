#ifndef SERVER_H
#define SERVER_H

extern sock_info_t          g_sock_info;
extern map<string,int>      g_name_sock;
extern map<int,string>      g_sock_name;
extern fd_set               g_all_set;
extern int                  g_max_fd;

#endif
