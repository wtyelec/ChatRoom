#ifndef SERVER_H
#define SERVER_H

extern map<int,user_info_t> g_user_info;
extern sock_info_t          g_sock_info;
extern map<string,int>      g_name_sock;
extern map<int,string>      g_sock_name;
extern map<int,string>      g_private_sock;

#endif
