#ifndef _BC_H_
#define _BC_H_

#include <string>
#include <netinet/in.h>
#include "userlist.h"
using std::string;
class broadcast
{
private:
    int optval;
    char buffer[1024];
    userlist ulist_impl;

public:
    broadcast();
    ~broadcast();
    void send(const string &message);
    void coding(char *buffer, unsigned int cmd, char *append);
    void bc();
};

#endif
