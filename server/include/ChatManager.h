#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <string>
#include <map>
#include <set>
#include "SocketManager.h"

using namespace std;

class user_info_t
{
public:
    user_info_t()
    {
    }

    inline string get_name()
    {
        return m_name;
    }

    inline void set_name(char* name_)
    {
        m_name = name_;
    }

private:
    string m_name;
};

class chat_manager_t
{
public:
    virtual void send_message(int sid_);
    virtual void wait_cli_conn();
};

#endif