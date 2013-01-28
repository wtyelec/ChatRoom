#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"

using namespace std;

sock_info_t             g_sock_info;
map<string,int>         g_name_sock;
map<int,string>         g_sock_name;

int main(int argc, char* argv[])
{
    fd_set                  cli_fds;
    struct timeval          ticks;
    chat_manager_t          chat_manager;

    ticks.tv_sec = ticks.tv_usec = 0;
    g_sock_info.init();
    int fdp_max(g_sock_info.get_ser_sock());
    
    while(1)
    {
        FD_ZERO(&cli_fds);
        FD_SET(g_sock_info.get_ser_sock(), &cli_fds);

        for(map<int,string>::iterator it = g_sock_name.begin(); it != g_sock_name.end(); it++)
        {
            int sock = (*it).first;
            FD_SET(sock, &cli_fds);
            fdp_max = fdp_max < sock ? sock:fdp_max;
        }

        switch(select(fdp_max+1, &cli_fds, NULL, NULL, &ticks))
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
                        if(FD_ISSET((*it).first, &cli_fds))
                        {
                            chat_manager.send_message((*it).first);
                        }
                    }
                }
                if(FD_ISSET(g_sock_info.get_ser_sock(), &cli_fds))
                {
                    chat_manager.wait_cli_conn();
                }
        }
    }

    return 0;
}
