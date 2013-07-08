#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>

using namespace std;

class util
{
public:
    static string int_str(int int_);
    static char* localtime_str();
    static struct tm* get_local_time();
    static string get_cur_sec();
    static string get_cur_time();
};    

#endif
