#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class sock_info_t
{
private:
    int         m_ser_sock;
    socklen_t   m_len;
    struct sockaddr_in m_ser;
    struct sockaddr_in m_cli;
public:
    sock_info_t()
    {
        m_ser_sock = 0;
        m_len = 0;
    };

    inline int get_ser_sock()
    {
        return m_ser_sock;
    }

    inline sockaddr_in get_cli()
    {
        return m_cli;
    }

    virtual void init();   
    virtual int accept_cli();

};

#endif
