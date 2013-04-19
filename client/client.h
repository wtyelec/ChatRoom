#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

using namespace std;

enum chat_type 
{
    NAME = 1,
    ALL = 2,
    PRIVATE = 3 
};

struct net_packet_head
{
    int16_t     body_size;
    chat_type   m_chat_type;
};

struct net_packet
{
    net_packet_head head;
    const char*     body;
};

#endif
