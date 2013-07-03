#include <fstream>
#include <iostream>
#include <stdarg.h>
#include "log.h"
#include "util.h"

using namespace std;

void log::c_log(const char* msg)
{
    cout << "1 log begin" << endl;
    cout << msg << endl;
    const char file_name[] = "mylog.txt";
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
    cout << "1 log end" << endl;
}

void log::c_log(const string &msg)
{
    cout << "2 log begin" << endl;
    cout << msg << endl;
    const char file_name[] = "mylog.txt";
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
    cout << "2 log end" << endl;
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
    cout << buf << endl;
    c_log(buf);


    va_end(ap);
}
