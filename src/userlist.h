#ifndef _USERLIST_H_
#define _USERLIST_H_
#include <iostream>
#include <cstring>
#include <list>
#include <netinet/in.h>
#include "public.h"
#include "user.h"
using std::list, std::string;

class userlist
{
private:
public:
    userlist();
    ~userlist();
    bool hasUser(in_addr &sin_addr);
    int addUser(in_addr &sin_addr, string name, string hostname);
};

#endif
