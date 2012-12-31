#include <iostream>
#include "SocketManager.h"
#include "ChatManager.h"

using namespace std;

fd_set              fds;
struct timeval      timeout;
map<int,UserInfo>   userInfo_buf;
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
        for(map<int,UserInfo>::iterator it = userInfo_buf.begin(); it != userInfo_buf.end(); it++)
        {
            int sid = (*it).first;
            FD_SET(sid, &fds);
            fdp_max = fdp_max < sid ? sid:fdp_max;
        }

        switch(select(fdp_max+1, &fds, NULL, NULL, &timeout))
        {
            case -1:
                cout << "select error!" << endl;
                break;
            case 0:
                break;
            default:
                if(userInfo_buf.size() > 0)
                {
                    for(map<int,UserInfo>::iterator it = userInfo_buf.begin(); it != userInfo_buf.end(); it++)
                    {
                        if(FD_ISSET((*it).first, &fds))
                        {
                            chatManager.SendMessage((*it).first, userInfo_buf, sInfo);
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
