#include <fstream>
#include <iostream>
#include <stdarg.h>
#include "log.h"
#include "util.h"

using namespace std;

void log::write_log(int severity, const char* msg)
{
    const char *severity_str;

    switch (severity)
    {
        case _CR_LOG_DEBUG:
            severity_str = "debug";
            break;
        case _CR_LOG_MSG:
            severity_str = "msg";
            break;
        case _CR_LOG_WARN:
            severity_str = "warn";
            break;
        case _CR_LOG_ERR:
            severity_str = "err";
            break;
    }

    const char file_name[] = "mylog.txt";
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << severity_str << "]"<< "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
}

void log::cr_log(int severity, const char *fmt, va_list ap)
{
    char buf[1024];
    size_t len;
    if(fmt != NULL)
    {
        int buf_len = sizeof(buf);
        vsnprintf(buf, buf_len, fmt, ap);
	    buf[buf_len-1] = '\0';
    }
    else
    {
        buf[0] = '\0';
    }
    write_log(severity, buf);
}

void log::cr_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_DEBUG, fmt, ap);

    va_end(ap);
}

void log::cr_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_MSG, fmt, ap);

    va_end(ap);
}

void log::cr_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_WARN, fmt, ap);

    va_end(ap);
}

void log::cr_err(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_ERR, fmt, ap);

    va_end(ap);
}


