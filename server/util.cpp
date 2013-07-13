#include <stdio.h>
#include <time.h>
#include <string.h>
#include "util.h"

using namespace std;

string util::int_str(int int_)
{
    string str;
    char buf[10];
    sprintf(buf, "%d", int_);
    str = buf;

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
    string hour = p->tm_hour > 9 ? int_str(p->tm_hour) : ("0" + int_str(p->tm_hour)); 
    string min = p->tm_min > 9 ? int_str(p->tm_min) : ("0" + int_str(p->tm_min)); 
    string sec = p->tm_sec > 9 ? int_str(p->tm_sec) : ("0" + int_str(p->tm_sec)); 

    return hour + ":" + min + ":" + sec;
}
