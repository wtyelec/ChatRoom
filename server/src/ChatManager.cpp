#include <iostream>
#include "ChatManager.h"
#include "global.h"

using namespace std;

void chat_manager_t::send_message(int sid_)
{
    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    if(recv(sid_, buf_recv, sizeof(buf_recv), 0) > 0)
    {
        char recevier[10];
        char* msg_ptr;
        msg_ptr = strchr(buf_recv, ':');
        if(msg_ptr == NULL)
        {
            send(sid_, "message form error", 18, 0);
            return;
        }
        strncpy(recevier, buf_recv, msg_ptr - buf_recv);
        recevier[msg_ptr - buf_recv] = '\0'; 
        string sender = g_sock_name[sid_];
        cout << "recv data from " << sender << ": " << msg_ptr + 1 << endl;

        if(strcmp("all", recevier) == 0)
        {
            map<int,string> tmp(g_sock_name);
            tmp.erase(sid_);      
            for(map<int,string>::iterator it = tmp.begin(); it != tmp.end(); it++)
            {
                sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)";
                send((*it).first, sender.c_str(), 30, 0);
            }
        }
        else
        {
            int recevier_sock = (*g_name_sock.find(recevier)).second;
            if(recevier_sock != 0)
            {
                if(g_sock_name.size() > 1)
                {
                    sender = sender + ": " + (msg_ptr + 1);
                    send(recevier_sock, sender.c_str(), 30, 0);
                }
            }
            else
            {
                send(sid_, "target not exist", 16, 0);
            }
        }
    }
    else
    {
        cout << g_sock_name[sid_] << " offline" << endl;
        g_name_sock.erase(g_sock_name[sid_]);
        g_sock_name.erase(sid_);
        close(sid_);
        FD_CLR(sid_, &g_all_set);
    }
}

void chat_manager_t::wait_cli_conn()
{
    int conn_fd = g_sock_info.accept_cli();

    char conn_name[10];
    if(recv(conn_fd, conn_name, sizeof(conn_name), 0) > 0)
    {
        g_sock_name[conn_fd] = conn_name;
        g_name_sock[conn_name] = conn_fd;
        cout << "port:" << ntohs(g_sock_info.get_cli().sin_port) << " num:" << g_name_sock.size()  << endl;
        FD_SET(conn_fd, &g_all_set);
        g_max_fd = g_max_fd < conn_fd ? conn_fd:g_max_fd;
    }
    else
    {
        cout << "input name error" << endl;
        return;
    }
}
