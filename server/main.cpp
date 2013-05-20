#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"
#include "global.h"

using namespace std;

sock_info_t         g_sock_info;
map<string,int>     g_name_sock;
map<int,string>     g_sock_name;
fd_set              g_all_set;
int                 g_max_fd(0);
event               g_serv_ev;

int main(int argc, char* argv[])
{
	struct timeval      ticks;
	chat_manager_t      chat_manager;
	fd_set              cur_set;

    event_init();      //libevent: event_init

	g_sock_info.init();
    event_set(&g_serv_ev, g_sock_info.get_serv_sock(), EV_READ | EV_PERSIST, chat_manager_t::accept_cli, NULL);
    event_add(&g_serv_ev, 0);
    event_dispatch();

	return 0;
}
