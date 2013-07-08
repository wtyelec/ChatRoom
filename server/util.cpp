#include <time.h>
#include <string.h>
#include "util.h"

string util::int_str(int int_)
{
    string str;
    ostringstream oss;
    oss << int_;
    str = oss.str();

    return str;
}

char* util::localtime_str()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    char* ret = asctime(timeinfo);
    *(ret + strlen(ret) - 1) = '\0'; // remove '\n'

    return ret;
}

struct tm* util::get_local_time()
{
    time_t tt;
    time(&tt);
    struct tm *p;
    p=localtime(&tt);

    return p;
}

string util::get_cur_time()
{
    tm* p = get_local_time();

    return int_str(p->tm_year+1900) + "_" + int_str(p->tm_mon+1) + "_" + int_str(p->tm_mday) 
        + "_" + int_str(p->tm_hour);
}

string util::get_cur_sec()
{
    tm* p = get_local_time();

    return int_str(p->tm_hour) + ":" + int_str(p->tm_min) + ":" + int_str(p->tm_sec);
}
