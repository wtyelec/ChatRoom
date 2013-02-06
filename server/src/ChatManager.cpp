#include <iostream>
#include "ChatManager.h"
#include "global.h"

using namespace std;

bool static recv_usr_name(int conn_fd_);

void chat_manager_t::send_message(int conn_fd_)
{
    if(recv_usr_name(conn_fd_))
    {
        return;
    }

    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    if(recv(conn_fd_, buf_recv, sizeof(buf_recv), 0) > 0)
    {
        char recevier[10];
        char* msg_ptr;
        msg_ptr = strchr(buf_recv, ':');
        if(msg_ptr == NULL)
        {
            send(conn_fd_, "message form error", 18, 0);
            return;
        }
        strncpy(recevier, buf_recv, msg_ptr - buf_recv);
        recevier[msg_ptr - buf_recv] = '\0'; 
        string sender = g_sock_name[conn_fd_];
        cout << "recv data from " << sender << ": " << msg_ptr + 1 << endl;

        if(strcmp("all", recevier) == 0)
        {
            map<int,string> tmp(g_sock_name);
            tmp.erase(conn_fd_);      
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
                send(conn_fd_, "target not exist", 16, 0);
            }
        }
    }
    else
    {
        cout << g_sock_name[conn_fd_] << " offline" << endl;
        g_name_sock.erase(g_sock_name[conn_fd_]);
        g_sock_name.erase(conn_fd_);
        close(conn_fd_);
        FD_CLR(conn_fd_, &g_all_set);
    }
}


bool static recv_usr_name(int conn_fd_)
{
    if(g_sock_name[conn_fd_] == "") 
    {
        char conn_name[10];
        if(recv(conn_fd_, conn_name, sizeof(conn_name), 0) > 0)
        {
            g_sock_name[conn_fd_] = conn_name;
            g_name_sock[conn_name] = conn_fd_;
            cout << "current connect fd = " << conn_fd_ << "; connected number = " << g_sock_name.size() << endl;
        }
        else
        {
            cout << "input name error" << endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}
