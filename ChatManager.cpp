#include "ChatManager.h"
#include <iostream>

using namespace std;

void ChatManager::SendMessage(int sid, map<int,UserInfo>& uInfo_m, SocketInfo& sInfo)
{
    char buf_recv[1024];
    memset(buf_recv, 0, sizeof(buf_recv));
    int recv_err = recv(sid, buf_recv, sizeof(buf_recv), 0);
    if(recv_err > 0)
    {
        const char* name_m = uInfo_m[sid].name.c_str(); 
        cout << "recv data from " << name_m << ": " << buf_recv << endl;
        if(uInfo_m.size() > 1)
        {
            char name_c[30] = ""; 
            strcat(name_c, name_m);
            strcat(name_c, ": ");
            strcat(name_c, buf_recv);

            map<int,UserInfo> tmp;
            tmp = uInfo_m;
            tmp.erase(sid);         // 移除自己的sid
            SendMsgToClients(tmp, name_c);
        }
    }
    else
    {
        uInfo_m.erase(sid);
        cout << uInfo_m[sid].name << " leave the chatroom." << endl;
    }
}

void ChatManager::SendMsgToClients(map<int,UserInfo>& uInfo_m, const char* msg)
{
    for(map<int,UserInfo>::iterator it = uInfo_m.begin(); it != uInfo_m.end(); it++)
    {
        send((*it).first, msg, sizeof(*msg),0);
    }
}

void ChatManager::AcceptClient(map<int,UserInfo>& uInfo_m, SocketInfo& sInfo)
{
    int cSid = sInfo.AcceptSocket();
    cout << "port:" << ntohs(sInfo.cli.sin_port) << " num:" << uInfo_m.size() + 1 << endl;
    // 接受client用户名存入name_data
    char name_data[10];
    recv(cSid, name_data, sizeof(name_data), 0);
    UserInfo newUser;
    newUser.name = name_data;
    uInfo_m.insert(pair<int,UserInfo>(cSid, newUser)); 
}
