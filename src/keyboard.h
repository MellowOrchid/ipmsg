#ifndef _KB_H_
#define _KB_H_

#include <vector>
#include "user.h"
using std::vector;

class keyboard
{
private:
    int result;
    bool hasUser(string destU);
    char codingbuff[BUFF_SIZE], message[BUFFER_SIZE], br_ip[20];
    string dest;
    vector<string> names, cmds;

public:
    keyboard();
    ~keyboard();
    int kb_scan();
    void help_cmd();
    void users_cmd();
    void exit_cmd();
    void RFL_cmd();
    void SFL_cmd();
    void sendto_cmd(string cmd);
    void getfile_cmd(string cmd);
};

#endif
