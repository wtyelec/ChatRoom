#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

using namespace std;

enum packet_type 
{
    NAME = 1,
    HEART,
    ALL,
    PRIVATE,
    EXCEPTION 
};

struct net_packet_head
{
    int16_t     body_size;
    packet_type m_packet_type;
};

struct net_packet_body
{
    int16_t         recv_id;
    char receiver[4];
    char message[20];
};

struct net_packet
{
    net_packet_head head;
    net_packet_body body;
};

#endif
