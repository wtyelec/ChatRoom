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
        case _CR_LOG_INFO:
            severity_str = "info ";
            break;
        case _CR_LOG_WARN:
            severity_str = "warn ";
            break;
        case _CR_LOG_ERROR:
            severity_str = "error";
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

void log::cr_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_INFO, fmt, ap);

    va_end(ap);
}

void log::cr_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_WARN, fmt, ap);

    va_end(ap);
}

void log::cr_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_ERROR, fmt, ap);

    va_end(ap);
}
