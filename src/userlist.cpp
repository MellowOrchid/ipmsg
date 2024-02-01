#include <string>
#include <netinet/in.h>
#include "userlist.h"

userlist::userlist()
{
    ulist.clear();
}

userlist::~userlist() {}

bool userlist::hasUser(in_addr &sin_addr)
{
    for (auto &&i : ulist)
    {
        if (i.sin_addr.s_addr == sin_addr.s_addr)
        {
            return true;
        }
    }
    return false;
}

int userlist::addUser(in_addr &sin_addr, string name, string hostname)
{
    return 0;
}
