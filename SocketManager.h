#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class SocketInfo
{
public:
    int         sListen;
    socklen_t   iLen;
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
