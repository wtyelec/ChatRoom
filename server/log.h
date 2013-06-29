#ifndef LOG_H
#define LOG_H

#include <string>

using namespace std;

class log
{
public:
    static void c_log(const char* msg);
    static void c_log(const string &msg);
};

#endif
