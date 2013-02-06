#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"

using namespace std;

sock_info_t         g_sock_info;
map<string,int>     g_name_sock;
map<int,string>     g_sock_name;
fd_set              g_all_set;
int                 g_max_fd(0);

int main(int argc, char* argv[])
{
    struct timeval      ticks;
    chat_manager_t      chat_manager;
    fd_set              cur_set;

    g_sock_info.init();
    g_max_fd = g_sock_info.get_ser_sock();
    //ticks.tv_sec = ticks.tv_usec = 0;
    FD_ZERO(&g_all_set);
    FD_SET(g_sock_info.get_ser_sock(), &g_all_set); 

    while(1)
    {
        cur_set = g_all_set;

        switch(select(g_max_fd+1, &cur_set, NULL, NULL, NULL))
        {
            case -1:
                cout << "select error!" << endl;
                break;
            case 0:
                break;
            default:
                if(g_sock_name.size() > 0)
                {
                    for(map<int,string>::iterator it = g_sock_name.begin(); it != g_sock_name.end(); it++)
                    //for(auto& it:g_sock_name)
                    {
                        if(FD_ISSET((*it).first, &cur_set))
                        {
                            chat_manager.send_message((*it).first);
                        }
                    }
                }
                if(FD_ISSET(g_sock_info.get_ser_sock(), &cur_set))
                {
                    int conn_fd = g_sock_info.accept_cli();
                    g_sock_name[conn_fd] = "";
                    FD_SET(conn_fd, &g_all_set);
                    g_max_fd = g_max_fd < conn_fd ? conn_fd:g_max_fd;
                }
        }
    }

    return 0;
}
