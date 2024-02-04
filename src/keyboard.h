#ifndef _KB_H_
#define _KB_H_

#include <list>
#include "user.h"
using std::list;

class keyboard
{
private:
    string dest;
    char codingbuff[BUFF_SIZE], message[BUFFER_SIZE];
    int result;
    bool hasUser(string destU);

public:
    keyboard();
    ~keyboard();
    int *kb_scan();
    void help_cmd();
    void users_cmd();
    void exit_cmd();
    void sendto_cmd(string cmd);
};

#endif
