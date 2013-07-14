#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <string>
#include <map>
#include <set>

#include "socket_manager.h"
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
        static void on_read(int fd, short ev, void* arg);
        static void on_write(int fd, short ev, void* arg);
        static void on_recv_name(int fd, char* name);
        static void accept_cli(int fd, short ev, void* arg);
        static int packet_write(int write_fd_, string& body_, packet_type type_);
        static int packet_write(int write_fd_, void* body_, packet_type type_);
        static void clear_conn_fd(int conn_fd_);
};

#endif
