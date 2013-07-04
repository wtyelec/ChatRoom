#include <string.h>
#include <stdlib.h>

#include "chat_manager.h"
#include "log.h"
#include "util.h"

using namespace std;

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024

int read_count(0);

void chat_manager_t::on_read(int fd, short ev, void* arg)
{
    log::cr_debug("chat_manager_t.on_read(fd = %d) begin", fd);

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
        char *buf_body = new char[read_packet.head.body_size];
        memset(buf_body, 0, sizeof(buf_body));
        read_body_ret = read(fd, buf_body, read_packet.head.body_size);
        if(read_body_ret > 0)
        {
            if(read_packet.head.m_packet_type == NAME)
            {
                event *write_ev = (struct event*) malloc(sizeof(struct event));
                event_set(write_ev, fd, EV_WRITE, on_recv_name, (void*)buf_body);
                event_base_set(g_ev_base, write_ev);
                event_add(write_ev, NULL);
            }
            else
            {
                char receiver[11];
                char *msg_ptr;
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
                delete [] buf_body;
                if(read_packet.head.m_packet_type == ALL)
                {
                    sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)";
                    cout << sender << endl;
                    map<int, string> tmp(g_sock_name);
                    tmp.erase(fd);    // remove sender fd (do not send to self)  
                    for(map<int,string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                    {
                        packet_write((*it).first, sender, ALL);
                    }
                }
                /*
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
                */
            }
        }
        else
        {
            delete [] buf_body;
        }
    }

    if(read_head_ret <= 0 || read_body_ret <= 0)
    {
        cout << g_sock_name[fd] << " offline" << endl;
        g_name_sock.erase(g_sock_name[fd]);
        clear_conn_fd(fd);
    }

    log::cr_debug("chat_manager_t.on_read( fd = %d) end", fd);
}

void chat_manager_t::on_write(int fd, short ev, void* arg)
{
    log::cr_debug("chat_manager_t.on_write(fd = %d) begin", fd);
    char *buf = (char*)arg; 
    map<int, string> tmp(g_sock_name);
    tmp.erase(fd);    // remove sender fd (do not send to self)  
    for(map<int,string>::iterator it = tmp.begin();
            it != tmp.end(); it++)
    {
        packet_write((*it).first, buf, ALL);
    }
    delete buf;
    
    log::cr_debug("chat_manager_t.on_write(fd = %d) end", fd);
}

int chat_manager_t::packet_write(int fd, string& body_,
        packet_type type_)
{
    log::cr_debug("chat_manager_t.packet_write(fd = %d) begin", fd);

    net_packet write_packet;
    write_packet.head.body_size = body_.size() + 1;
    write_packet.head.m_packet_type = type_;
    write(fd, (char*)&write_packet.head, sizeof(net_packet_head));
    int body_ret = write(fd, body_.data(), body_.size() + 1);

    log::cr_debug("chat_manager_t.packet_write(fd = %d) end", fd);
    return body_ret;
}

int chat_manager_t::packet_write(int fd, void* body_, 
        packet_type type_)
{
    log::cr_debug("chat_manager_t.packet_write(fd = %d) begin", fd);

    char *buf = (char*) body_;
    net_packet write_packet;
    write_packet.head.body_size = strlen(buf);
    write_packet.head.m_packet_type = type_;
    write(fd, (char*)&write_packet.head, sizeof(net_packet_head));
    int body_ret = write(fd, buf, strlen(buf) + 1);

    log::cr_debug("chat_manager_t.packet_write(fd = %d) end", fd);
    return body_ret;
}

void chat_manager_t::on_recv_name(int fd, short ev, void* arg)
{
    log::cr_debug("chat_manager_t.on_recv_name(fd = %d) begin", fd);

    char *name = (char*)arg; 
    if(g_name_sock.find(name) == g_name_sock.end())
    {
        g_sock_name[fd] = name;
        g_name_sock[name] = fd;
        string msg = "Please start to chat!";
        packet_write(fd, msg, ALL);
        cout << g_sock_name[fd] << " enter chat room" << endl;
    }
    else
    {
        string err_same_name = "The name has used, please input another!";
        packet_write(fd, err_same_name, EXCEPTION);
    }
    delete [] name;

    log::cr_debug("chat_manager_t.on_recv_name(fd = %d) end", fd);
}

void chat_manager_t::clear_conn_fd(int fd)
{
    log::cr_debug("chat_manager_t.clear_conn_fd(fd = %d) begin", fd);

	g_sock_name.erase(fd);
	close(fd);
	FD_CLR(fd, &g_all_set);
    
    log::cr_debug("chat_manager_t.clear_conn_fd(fd = %d) end", fd);
}

void chat_manager_t::accept_cli(int fd, short ev, void* arg)
{
    log::cr_debug("chat_manager_t.accept_cli(fd = %d) begin", fd);

    struct sockaddr_in  cli_addr;
    socklen_t cli_addr_len;
    cli_addr_len = sizeof(cli_addr);
    int cli_fd = accept(fd, (struct sockaddr *)&cli_addr,
            &cli_addr_len);

    cout << "port:" << ntohs(cli_addr.sin_port)
        << "; current connect fd = " << cli_fd
        << "; connected number = " << g_sock_name.size() + 1
        << endl;

    g_sock_name[cli_fd] = "";

    event *read_ev = (struct event*)malloc(sizeof(struct event));
    event_set(read_ev, cli_fd, EV_READ | EV_PERSIST, on_read,
            read_ev);
    event_base_set(g_ev_base, read_ev);
    event_add(read_ev, NULL);

    log::cr_debug("chat_manager_t.accept_cli(fd = %d) end", fd);
}
