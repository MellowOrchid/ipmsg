#ifndef _RL_H_
#define _RL_H_

#include <iostream>
#include <string>
using std::string;

class wlog
{
private:
public:
    wlog();
    ~wlog();
    static void log(string message);
    static void log(const char *message);
    static void log(int message);
    static void log(string message, int append);
};

#endif
