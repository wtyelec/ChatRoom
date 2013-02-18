#include "ChatManager.h"
#include "global.h"

using namespace std;

uint8_t static recv_usr_name(int16_t conn_fd_);
void static clear_conn_fd(int16_t conn_fd_);

void chat_manager_t::send_message(int16_t conn_fd_)
{
	if(recv_usr_name(conn_fd_) == 1)
	{
		return;
	}

	char buf_recv[1024];
	memset(buf_recv, 0, sizeof(buf_recv));
	int16_t recv_len = recv(conn_fd_, buf_recv, sizeof(buf_recv), 0);
	if(recv_len > 0)
	{
		char recevier[10];
		char* msg_ptr;
		msg_ptr = strchr(buf_recv, ':');
		if(msg_ptr == NULL)
		{
			char err_input_format[] = "input format wrong, please input correctly!"; 
			send(conn_fd_, err_input_format, sizeof(err_input_format), 0);
			return;
		}
		strncpy(recevier, buf_recv, msg_ptr - buf_recv);
		recevier[msg_ptr - buf_recv] = '\0'; 
		string sender = g_sock_name[conn_fd_];
		cout << sender << ": " << msg_ptr + 1 << endl;

		if(strcmp("all", recevier) == 0)
		{
			map<int16_t,string> tmp(g_sock_name);
			tmp.erase(conn_fd_);      
			for(map<int16_t,string>::iterator it = tmp.begin(); it != tmp.end(); it++)
			{
				sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)";
				send((*it).first, sender.c_str(), 30, 0);
			}
		}
		else
		{
			int16_t recevier_sock = (*g_name_sock.find(recevier)).second;
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
		clear_conn_fd(conn_fd_);
	}
}

uint8_t static recv_usr_name(int16_t conn_fd_)
{
	if(g_sock_name[conn_fd_] == "") 
	{
		char conn_name[10];
		if(recv(conn_fd_, conn_name, sizeof(conn_name), 0) > 0)
		{
			if(g_name_sock.find(conn_name) == g_name_sock.end())
			{
				g_sock_name[conn_fd_] = conn_name;
				g_name_sock[conn_name] = conn_fd_;
				char msg[] = "please start to chat!";
				send(conn_fd_, msg, sizeof(msg), 0);
			}
			else
			{
				char err_same_name[] = "the name has used, please input another!";
				send(conn_fd_, err_same_name, sizeof(err_same_name), 0); 
			}
		}
		else
		{
			cout << "connected but not input name" << endl;
			clear_conn_fd(conn_fd_);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void static clear_conn_fd(int16_t conn_fd_)
{
	g_sock_name.erase(conn_fd_);
	close(conn_fd_);
	FD_CLR(conn_fd_, &g_all_set);
}
