#include "ChatManager.h"
#include "global.h"

using namespace std;

void static recv_usr_name(int16_t conn_fd_, const char* name_);
void static clear_conn_fd(int16_t conn_fd_);
int16_t static packet_write(int16_t write_fd_, string& body_, packet_type type_);

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024

int16_t read_count(0);

void chat_manager_t::send_message(int16_t conn_fd_)
{
    // read net_packet_head
    char buf_head[sizeof(net_packet_head)];
    memset(buf_head, 0, sizeof(buf_head));
    int16_t read_head_ret = read(conn_fd_, buf_head, sizeof(buf_head));
    int16_t read_body_ret(0);
    if(read_head_ret > 0)
    {
        net_packet read_packet;
        read_packet.head = *(net_packet_head*)buf_head;
        // read net_packet_body
        char buf_body[read_packet.head.body_size];
        memset(buf_body, 0, sizeof(buf_body));
        read_body_ret = read(conn_fd_, buf_body, read_packet.head.body_size);
        if(read_body_ret > 0)
        {
            if(read_packet.head.m_packet_type == NAME)
            {
                recv_usr_name(conn_fd_, buf_body);
            }
            else
            {
                char receiver[11];
                char* msg_ptr;
                msg_ptr = strchr(buf_body, ':');
                if(msg_ptr == NULL)
                {
                    string err_input_format = "input format wrong, please input correctly!"; 
                    packet_write(conn_fd_, err_input_format, EXCEPTION);
                    cout << "recv wrong format message:" << buf_body << endl;
                    return;
                }
                // analysis data
                strncpy(receiver, buf_body, msg_ptr - buf_body);
                receiver[msg_ptr - buf_body] = '\0'; 
                string sender = g_sock_name[conn_fd_];

                if(read_packet.head.m_packet_type == ALL)
                {
                    sender = sender + ": " + (msg_ptr + 1) + " (in a chat room)";
                    cout << sender << "count: " << read_count++ << endl; 
                    map<int16_t,string> tmp(g_sock_name);
                    tmp.erase(conn_fd_);    // remove sender fd   
                    for(map<int16_t,string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                    {
                        packet_write((*it).first, sender, ALL);
                    }
                }
                else if(read_packet.head.m_packet_type == HEART)
                {
                    string heart_beat = "heart:beat";
                    packet_write(conn_fd_, heart_beat, HEART);
                }
                else if(read_packet.head.m_packet_type == PRIVATE)
                {
                    sender = sender + ": " + (msg_ptr + 1) + "(private)"; 
                    cout << sender << endl; 
                    int16_t write_fd = (*g_name_sock.find(receiver)).second;
                    if(write_fd != 0)
                    {
                        packet_write(write_fd, sender, PRIVATE);
                    }
                    else
                    {
                        string target_not_exist = "target not exist"; 
                        packet_write(conn_fd_, target_not_exist, EXCEPTION); 
                    }
                }
            }
        }
    }
    if(read_head_ret <= 0 || read_body_ret <= 0)
    {
        cout << g_sock_name[conn_fd_] << " offline" << endl;
        g_name_sock.erase(g_sock_name[conn_fd_]);
        clear_conn_fd(conn_fd_);
    }
}

int16_t static packet_write(int16_t write_fd_, string& body_, packet_type type_)
{
    net_packet write_packet;
    write_packet.head.body_size = body_.size() + 1;
    write_packet.head.m_packet_type = type_;
    write(write_fd_, (char*)&write_packet.head, sizeof(net_packet_head));
    int16_t body_ret = write(write_fd_, body_.data(), body_.size() + 1);

    return body_ret;
}

void static recv_usr_name(int16_t conn_fd_, const char* name_)
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

void static clear_conn_fd(int16_t conn_fd_)
{
	g_sock_name.erase(conn_fd_);
	close(conn_fd_);
	FD_CLR(conn_fd_, &g_all_set);
}
