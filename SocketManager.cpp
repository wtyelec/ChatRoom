#include <iostream>
#include "SocketManager.h"

using namespace std;

void sock_info_t::init()
{
    m_ser.sin_family = AF_INET;
    m_ser.sin_addr.s_addr = INADDR_ANY;
    m_ser.sin_port = htons(6666);
    m_ser_sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(m_ser_sock, (struct sockaddr*)&m_ser, sizeof(m_ser));
    listen(m_ser_sock, 5);
    m_len = sizeof(m_cli);
    cout << "server init succeed" << endl;
}

int sock_info_t::accept_cli()
{
    return accept(m_ser_sock, (struct sockaddr *)&m_cli, &m_len);
}
