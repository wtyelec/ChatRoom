#include <iostream>
#include "ChatManager.h"
#include "server.h"

using namespace std;

void chat_manager_t::group_send(int sid_)
{
    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    if(recv(sid_, buf_recv, sizeof(buf_recv), 0) > 0)
    {
        string msg = g_user_info[sid_].get_name(); 
        cout << "recv data from " << msg << ": " << buf_recv << endl;
        if(g_user_info.size() > 1)
        {
            msg = msg + ": " + buf_recv + " (in a chat room)";
            map<int,user_info_t> tmp(g_user_info);
            tmp.erase(sid_);      
            for(map<int,user_info_t>::iterator it = tmp.begin(); it != tmp.end(); it++)
            {
                send((*it).first, msg.c_str(), 30, 0);
            }
        }
    }
    else
    {
        cout << g_user_info[sid_].get_name() << " leave the chatroom." << endl;
        g_user_info.erase(sid_);
        g_name_sock.erase(g_sock_name[sid_]);
        g_sock_name.erase(sid_);
    }
}

void chat_manager_t::private_send(int sid_)
{
    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    if(recv(sid_, buf_recv, sizeof(buf_recv), 0) > 0)
    {
        string msg = g_sock_name[sid_];
        cout << "recv data from " << msg << ": " << buf_recv << endl;
        int target_sock = g_name_sock[g_private_sock[sid_]]; 
        if(g_private_sock.size() > 0)
        {
            msg = msg + ": " + buf_recv;
            send(target_sock, msg.c_str(), 30, 0);
        }
    }
    else
    {
        cout << g_sock_name[sid_] << " offline" << endl;
        g_private_sock.erase(sid_);
        g_name_sock.erase(g_sock_name[sid_]);
        g_sock_name.erase(sid_);
    }
}

void chat_manager_t::wait_cli_conn()
{
    int cli_sock = g_sock_info.accept_cli();

    char conn_name[10];
    if(recv(cli_sock, conn_name, sizeof(conn_name), 0) > 0)
    {
        g_sock_name[cli_sock] = conn_name;
        g_name_sock[conn_name] = cli_sock;
        cout << "port:" << ntohs(g_sock_info.get_cli().sin_port) << " num:" << g_name_sock.size()  << endl;
    }
    else
    {
        cout << "input name error" << endl;
        return;
    }

    char target_name[10];
    if(recv(cli_sock, target_name, sizeof(target_name), 0) > 0)
    {
        if(strcmp("all",target_name) == 0)
        {
            user_info_t newUser;
            newUser.set_name(conn_name);
            g_user_info[cli_sock] = newUser;
        }            
        else
        {
            int target_id = (*g_name_sock.find(target_name)).second; 
            if(target_id != 0)
            {
                g_private_sock[cli_sock] = target_name; 
            }
        }
    }
    else
    {
        cout << "input target error" << endl;
        g_name_sock.erase(g_sock_name[cli_sock]);
        g_sock_name.erase(cli_sock);
    }
}
