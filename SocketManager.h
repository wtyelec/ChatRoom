#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <set>

using namespace std;

class SocketInfo
{
public:
    int         sListen;
    socklen_t   iLen;
    set<int>    socket_s;
    struct sockaddr_in ser;
    struct sockaddr_in cli;

    SocketInfo()
    {
        sListen = 0;
        iLen = 0;
    };

    virtual void Init();   
    virtual int AcceptSocket();
};

#endif
