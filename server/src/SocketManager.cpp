#include <iostream>
#include "SocketManager.h"

using namespace std;

void sock_info_t::init()
{
    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_addr.s_addr = INADDR_ANY;
    m_serv_addr.sin_port = htons(6666);
    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt =1;
    setsockopt(m_listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(m_listen_fd, (struct sockaddr*)&m_serv_addr, sizeof(m_serv_addr));
    listen(m_listen_fd, 5);
    m_len = sizeof(m_cli_addr);
    cout << "server init succeed" << endl;
}

int sock_info_t::accept_cli()
{
    return accept(m_listen_fd, (struct sockaddr *)&m_cli_addr, &m_len);
}
