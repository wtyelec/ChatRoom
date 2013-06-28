#include "util.h"

string util::int_str(int int_)
{
    string str;
    ostringstream oss;
    oss << int_;
    str = oss.str();

    return str;
}
