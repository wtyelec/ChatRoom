#include <fstream>
#include <stdarg.h>
#include "log.h"
#include "util.h"

using namespace std;

void log::write_log(const char *msg)
{
    const char file_name[] = "mylog.txt";
    fstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
}

void log::write_log(string msg)
{
    const char file_name[] = "mylog.txt";
    fstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
}

void log::cr_log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

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

    va_end(ap);
}
