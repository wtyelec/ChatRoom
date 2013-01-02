#include <iostream>
#include "ChatManager.h"
#include "server.h"

using namespace std;

void chat_manager_t::send_msg(int sid_)
{
    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    if(recv(sid_, buf_recv, sizeof(buf_recv), 0) > 0)
    {
        string msg = g_user_info[sid_].get_name(); 
        cout << "recv data from " << msg << ": " << buf_recv << endl;
        if(g_user_info.size() > 1)
        {
            msg = msg + ": " + buf_recv;
            map<int,user_info_t> tmp(g_user_info);
            tmp.erase(sid_);         // 移除自己的sid
            group_send(tmp, msg.c_str());
        }
    }
    else
    {
        cout << g_user_info[sid_].get_name() << " leave the chatroom." << endl;
        g_user_info.erase(sid_);
    }
}

void chat_manager_t::group_send(map<int,user_info_t>& user_info_, const char* msg_)
{
    for(map<int,user_info_t>::iterator it = user_info_.begin(); it != user_info_.end(); it++)
    {
        send((*it).first, msg_, 30, 0);
    }
}

void chat_manager_t::wait_cli_conn()
{
    int cli_sock = g_sock_info.accept_cli();
    cout << "port:" << ntohs(g_sock_info.get_cli().sin_port) << " num:" << g_user_info.size() + 1 << endl;
    // 接受client用户名存入name_data
    char name_data[10];
    recv(cli_sock, name_data, sizeof(name_data), 0);
    user_info_t newUser;
    newUser.set_name(name_data);
    g_user_info[cli_sock] = newUser;
}
