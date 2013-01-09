#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"

using namespace std;

fd_set                  g_fds;
struct timeval          g_timeout;
sock_info_t             g_sock_info;
map<string,int>         g_name_sock;
map<int,string>         g_sock_name;
chat_manager_t          g_chat_manager;

int main(int argc, char* argv[])
{
    g_timeout.tv_sec = g_timeout.tv_usec = 0;
    g_sock_info.init();
    int fdp_max(g_sock_info.get_ser_sock());
    
    while(1)
    {
        FD_ZERO(&g_fds);
        FD_SET(g_sock_info.get_ser_sock(), &g_fds);

        for(map<int,string>::iterator it = g_sock_name.begin(); it != g_sock_name.end(); it++)
        {
            int sock = (*it).first;
            FD_SET(sock, &g_fds);
            fdp_max = fdp_max < sock ? sock:fdp_max;
        }

        switch(select(fdp_max+1, &g_fds, NULL, NULL, &g_timeout))
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
                    {
                        if(FD_ISSET((*it).first, &g_fds))
                        {
                            g_chat_manager.send_message((*it).first);
                        }
                    }
                }
                if(FD_ISSET(g_sock_info.get_ser_sock(), &g_fds))
                {
                    g_chat_manager.wait_cli_conn();
                }
        }
    }

    return 0;
}