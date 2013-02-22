#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"
#include "fcntl.h"

using namespace std;

sock_info_t         	g_sock_info;
map<string,int16_t>     g_name_sock;
map<int16_t,string>     g_sock_name;
fd_set              	g_all_set;
int16_t                 g_max_fd(0);

int main(int argc, char* argv[])
{
	struct timeval      ticks;
	chat_manager_t      chat_manager;
	fd_set              cur_set;
	fd_set				exc_set;

	g_sock_info.init();
	g_max_fd = g_sock_info.get_ser_sock();
	ticks.tv_sec = 5;
	ticks.tv_usec = 0;
	FD_ZERO(&g_all_set);
	FD_SET(g_sock_info.get_ser_sock(), &g_all_set); 

	while(1)
	{
		cur_set = g_all_set;
		exc_set = g_all_set;

		switch(select(g_max_fd+1, &cur_set, NULL, &exc_set, &ticks))
		{
			case -1:
				cout << "select error!" << endl;
				break;
			case 0:
				break;
			default:
				if(g_sock_name.size() > 0)
				{
					for(map<int16_t,string>::iterator it = g_sock_name.begin(); it != g_sock_name.end(); it++)
					{
						if(FD_ISSET((*it).first, &cur_set))
						{
							chat_manager.send_message((*it).first);
						}
					}
				}
				if(FD_ISSET(g_sock_info.get_ser_sock(), &cur_set))
				{
					g_sock_info.accept_cli();
				}
		}
	}

	return 0;
}
