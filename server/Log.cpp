#include <fstream>
#include "log.h"

using namespace std;

void log::c_log(const char* msg)
{
    const char file_name[] = "mylog.txt";
    ofstream o_file;
    o_file.open(file_name);
    o_file << msg << endl;
    o_file.close();
}

void log::c_log(const string &msg)
{
    const char file_name[] = "mylog.txt";
    ofstream o_file;
    o_file.open(file_name);
    o_file << msg << endl;
    o_file.close();
}
