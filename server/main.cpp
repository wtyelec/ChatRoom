#include <iostream>
#include "socket_manager.h"
#include "chat_manager.h"
#include "global.h"
#include "assert.h"

using namespace std;

sock_info_t         g_sock_info;
map<string,int>     g_name_sock;
map<int,string>     g_sock_name;
fd_set              g_all_set;
int                 g_max_fd(0);
event               g_serv_ev;
event               g_cli_ev;
event_base*         g_ev_base;

int main(int argc, char* argv[])
{
	struct timeval      ticks;
	chat_manager_t      chat_manager;
	fd_set              cur_set;

	g_sock_info.init();
    int serv_sock = g_sock_info.get_serv_sock();
    g_ev_base = event_base_new(); 
    assert(g_ev_base != NULL);
    event_set(&g_serv_ev, serv_sock, EV_READ | EV_PERSIST, chat_manager_t::accept_cli, NULL);
    event_base_set(g_ev_base, &g_serv_ev);
    event_add(&g_serv_ev, NULL);

    event_base_dispatch(g_ev_base);

	return 0;
}
