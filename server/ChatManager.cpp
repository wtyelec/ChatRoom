#include "ChatManager.h"

using namespace std;

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024

int read_count(0);

void chat_manager_t::send_message(int fd, short ev, void* arg)
{
    // read net_packet_head
    char buf_head[sizeof(net_packet_head)];
    memset(buf_head, 0, sizeof(buf_head));
    int read_head_ret = read(fd, buf_head, sizeof(buf_head));
    int read_body_ret(0);
    if(read_head_ret > 0)
    {
        net_packet read_packet;
        read_packet.head = *(net_packet_head*)buf_head;
        // read net_packet_body
        char buf_body[read_packet.head.body_size];
        memset(buf_body, 0, sizeof(buf_body));
        read_body_ret = read(fd, buf_body, read_packet.head.body_size);
        if(read_body_ret > 0)
        {
            if(read_packet.head.m_packet_type == NAME)
            {
                recv_usr_name(fd, buf_body);
            }
            else
            {
                char receiver[11];
                char* msg_ptr;
                msg_ptr = strchr(buf_body, ':');
                if(msg_ptr == NULL)
                {
                    string err_input_format = "input format wrong, please input correctly!"; 
                    packet_write(fd, err_input_format, EXCEPTION);
                    cout << "recv wrong format message:" << buf_body << endl;
                    return;
                }
                // analysis data
                strncpy(receiver, buf_body, msg_ptr - buf_body);
                receiver[msg_ptr - buf_body] = '\0'; 
                string sender = g_sock_name[fd];

                if(read_packet.head.m_packet_type == ALL)
                {
                    sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)";
                    cout << sender << "count: " << read_count++ << endl; 
                    map<int,string> tmp(g_sock_name);
                    tmp.erase(fd);    // remove sender fd   
                    for(map<int,string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                    {
                        packet_write((*it).first, sender, ALL);
                    }
                }
                else if(read_packet.head.m_packet_type == HEART)
                {
                    string heart_beat = "heart:beat";
                    packet_write(fd, heart_beat, HEART);
                }
                else if(read_packet.head.m_packet_type == PRIVATE)
                {
                    sender = sender + ": " + (msg_ptr + 1) + "(private)"; 
                    cout << sender << endl; 
                    int write_fd = (*g_name_sock.find(receiver)).second;
                    if(write_fd != 0)
                    {
                        packet_write(write_fd, sender, PRIVATE);
                    }
                    else
                    {
                        string target_not_exist = "target not exist"; 
                        packet_write(fd, target_not_exist, EXCEPTION); 
                    }
                }
            }
        }
    }
    if(read_head_ret <= 0 || read_body_ret <= 0)
    {
        cout << g_sock_name[fd] << " offline" << endl;
        g_name_sock.erase(g_sock_name[fd]);
        clear_conn_fd(fd);
    }
}

int chat_manager_t::packet_write(int write_fd_, string& body_, packet_type type_)
{
    net_packet write_packet;
    write_packet.head.body_size = body_.size() + 1;
    write_packet.head.m_packet_type = type_;
    write(write_fd_, (char*)&write_packet.head, sizeof(net_packet_head));
    int body_ret = write(write_fd_, body_.data(), body_.size() + 1);

    return body_ret;
}

void chat_manager_t::recv_usr_name(int conn_fd_, const char* name_)
{
    if(g_name_sock.find(name_) == g_name_sock.end())
    {
        g_sock_name[conn_fd_] = name_;
        g_name_sock[name_] = conn_fd_;
        string msg = "Please start to chat!";
        packet_write(conn_fd_, msg, ALL);
        cout << g_sock_name[conn_fd_] << " enter chat room" << endl;
    }
    else
    {
        string err_same_name = "The name has used, please input another!";
        packet_write(conn_fd_, err_same_name, EXCEPTION);
    }
}

void chat_manager_t::clear_conn_fd(int conn_fd_)
{
	g_sock_name.erase(conn_fd_);
	close(conn_fd_);
	FD_CLR(conn_fd_, &g_all_set);
}

void chat_manager_t::accept_cli(int serv_fd, short ev, void* arg)
{
    cout << "accept_cli begin" << endl;
    struct sockaddr_in  cli_addr;
    socklen_t cli_addr_len;
    cli_addr_len = sizeof(cli_addr);
    int cli_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    cout << "port:" << ntohs(cli_addr.sin_port) << "; current connect fd = " << cli_fd << "; connected number = " << g_sock_name.size() + 1 << endl;
    log::log_current_time();
    g_sock_name[cli_fd] = "";
    //event_set(&g_serv_ev, cli_fd, EV_READ | EV_PERSIST, send_message, NULL);
    //event_set(&g_serv_ev, g_sock_info.get_serv_sock(), EV_READ | EV_PERSIST, accept_cli, NULL);
    //event_add(&g_serv_ev, 0);
}
