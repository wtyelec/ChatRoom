#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <string>
#include <map>
#include <set>
#include "SocketManager.h"
#include "global.h"

using namespace std;

class user_info_t
{
	public:
		user_info_t()
		{
		}

		inline string get_name()
		{
			return m_name;
		}

		inline void set_name(char* name_)
		{
			m_name = name_;
		}
	private:
		string m_name;
};

class chat_manager_t
{
	public:
        static void send_message(int fd, short ev, void* arg);
        static void accept_cli(int fd, short ev, void* arg);
        static int packet_write(int write_fd_, string& body_, packet_type type_);
        static void recv_usr_name(int conn_fd_, const char* name_);
        static void clear_conn_fd(int conn_fd_);
};

#endif
