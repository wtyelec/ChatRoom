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
