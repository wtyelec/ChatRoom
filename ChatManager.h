#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <string>
#include <map>
#include "SocketManager.h"

using namespace std;

class UserInfo
{
public:
    string name;

    UserInfo()
    {
    }
};

class ChatManager
{
public:
    virtual void SendMessage(int sid, map<int,UserInfo>& uInfo_m, SocketInfo& sInfo);
    virtual void AcceptClient(map<int,UserInfo>& uInfo_m, SocketInfo& sInfo);
    static void SendMsgToClients(const set<int>& ids, const char* msg);
};

#endif
