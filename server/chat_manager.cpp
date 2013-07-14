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
                on_recv_name(fd, buf_body);
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
                string sender = g_fd_info[fd].name;
                if(read_packet.head.m_packet_type == ALL)
                {
                    sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)" + util::int_str(++read_count);
                    cout << sender << endl;
                    log::cr_info(sender.data());
                    map<int, fd_info> tmp(g_fd_info);
                    tmp.erase(fd);    // remove sender fd (do not send to self)  
                    for(map<int,fd_info>::iterator it = tmp.begin(); it != tmp.end(); it++)
                    {
                        packet_write((*it).first, sender, ALL);
                    }
                }
                delete [] buf_body;
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
        cout << g_fd_info[fd].name << " offline" << endl;
        g_name_fd.erase(g_fd_info[fd].name);
        event_del(g_fd_info[fd].ev);
        g_fd_info.erase(fd);
        close(fd);
    }

    log::cr_debug("chat_manager_t.on_read( fd = %d) end", fd);
}

void chat_manager_t::on_write(int fd, short ev, void* arg)
{
    log::cr_debug("chat_manager_t.on_write(fd = %d) begin", fd);
    char *buf = (char*)arg; 
    map<int, fd_info> tmp(g_fd_info);
    tmp.erase(fd);    // remove sender fd (do not send to self)  
    for(map<int,fd_info>::iterator it = tmp.begin();
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

void chat_manager_t::on_recv_name(int fd, char* name)
{
    log::cr_debug("chat_manager_t.on_recv_name(fd = %d) begin", fd);

    if(g_name_fd.find(name) == g_name_fd.end())
    {
        g_fd_info[fd].name = name;
        g_name_fd[name] = fd;
        string msg = "Please start to chat!";
        packet_write(fd, msg, ALL);
        cout << g_fd_info[fd].name << " enter chatroom" << endl;
        log::cr_info("chat_manager_t.on_recv_name(): %s enter chatroom.", g_fd_info[fd].name.data());
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

	g_fd_info.erase(fd);
	close(fd);
    
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

    log::cr_info("chat_manager_t.accept_cli(port = %d, cli_fd = %d, num = %d)", 
            ntohs(cli_addr.sin_port), cli_fd, g_fd_info.size() + 1);

    g_fd_info[cli_fd].name = "";

    event *read_ev = (struct event*)malloc(sizeof(struct event));
    event_set(read_ev, cli_fd, EV_READ | EV_PERSIST, on_read,
            read_ev);
    event_base_set(g_ev_base, read_ev);
    event_add(read_ev, NULL);
    g_fd_info[cli_fd].ev = read_ev;

    log::cr_debug("chat_manager_t.accept_cli(fd = %d) end", fd);
}
