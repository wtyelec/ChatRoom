#include <iostream>
#include "SocketManager.h"

using namespace std;

void SocketInfo::Init()
{
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = INADDR_ANY;
    ser.sin_port = htons(6666);
    sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (struct sockaddr*)&ser, sizeof(ser));
    listen(sListen, 5);
    iLen = sizeof(cli);
    cout << "server init succeed" << endl;
}

int SocketInfo::AcceptSocket()
{
    int cSid = accept(sListen, (struct sockaddr *)&cli, &iLen);
    return cSid; 
}
