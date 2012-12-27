#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "SocketManager.h"
#include "ChatManager.h"

using namespace std;

void send_msg(int sid);
void accept_client();
void server_socket();

fd_set              fds;
map<int,UserInfo>   userInfo_buf;
struct timeval      timeout;
SocketInfo          sInfo;
ChatManager         chatManager;

int main(int argc, char* argv[])
{
    timeout.tv_sec = timeout.tv_usec = 0;
    sInfo.Init();
    int fdp_max(sInfo.sListen);

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(sInfo.sListen, &fds);
        for(set<int>::iterator it = sInfo.socket_s.begin(); it != sInfo.socket_s.end(); it++)
        {
            FD_SET(*it, &fds);
            fdp_max = fdp_max < *it ? *it:fdp_max;
        }

        switch(select(fdp_max+1, &fds, NULL, NULL, &timeout))
        {
            case -1:
                cout << "select error!" << endl;
                break;
            case 0:
                break;
            default:
                if(sInfo.socket_s.size() > 0)
                {
                    for(set<int>::iterator it = sInfo.socket_s.begin(); it != sInfo.socket_s.end(); it++)
                    {
                        if(FD_ISSET(*it, &fds))
                        {
                            chatManager.SendMessage(*it, userInfo_buf, sInfo);
                        }
                    }
                }
                if(FD_ISSET(sInfo.sListen, &fds))
                {
                    chatManager.AcceptClient(userInfo_buf, sInfo);
                }
        }
    }

    return 0;
}
