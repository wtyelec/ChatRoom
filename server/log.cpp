#include <fstream>
#include "log.h"
#include "util.h"

using namespace std;

void log::c_log(const char* msg)
{
    const char file_name[] = "mylog.txt";
    fstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
}

void log::c_log(const string &msg)
{
    const char file_name[] = "mylog.txt";
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << util::localtime_str() << "] " << msg << endl;
    fs.close();
}
