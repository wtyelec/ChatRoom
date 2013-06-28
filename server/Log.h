#ifndef LOG_H
#define LOG_H

#include <string>

using namespace std;

class log
{
public:
    /*
    inline static void log_current_time()
    {
        time_t ticks;
        time(&ticks);
        cout << "time: " << ctime(&ticks);
    }
*/
    static void c_log(const char* msg);
    static void c_log(const string &msg);
};

#endif
