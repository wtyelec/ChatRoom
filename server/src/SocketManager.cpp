#include <iostream>
#include "SocketManager.h"
#include "global.h"

using namespace std;

void sock_info_t::init()
{
	m_serv_addr.sin_family = AF_INET;
	m_serv_addr.sin_addr.s_addr = INADDR_ANY;
	m_serv_addr.sin_port = htons(6666);
	m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(m_listen_fd,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bind(m_listen_fd, (struct sockaddr*)&m_serv_addr, sizeof(m_serv_addr));
	listen(m_listen_fd, 5);
	m_len = sizeof(m_cli_addr);
	cout << "server init succeed" << endl;
}

int16_t sock_info_t::accept_cli()
{
	int16_t conn_fd = accept(m_listen_fd, (struct sockaddr *)&m_cli_addr, &m_len);
	cout << "port:" << ntohs(get_cli().sin_port) << "; current connect fd = " << conn_fd << "; connected number = " << g_sock_name.size() + 1 << endl;
	log::log_current_time();
	g_sock_name[conn_fd] = "";
	FD_SET(conn_fd, &g_all_set);
	g_max_fd = g_max_fd < conn_fd ? conn_fd:g_max_fd;

	return conn_fd;
}
