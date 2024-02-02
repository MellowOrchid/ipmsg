#include <string>
#include <netinet/in.h>
#include "userlist.h"
#include "user.h"

userlist::userlist()
{
    // ulist.clear();
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
    user u(sin_addr, name, hostname);
    ulist.push_back(u);
    return 0;
}

int userlist::delUser(in_addr sin_addr)
{
    if (ulist.empty())
    {
        return 0;
    }

    for (auto &&i : ulist)
    {
        if (i.sin_addr.s_addr == sin_addr.s_addr)
        {
            ulist.remove(i);
        }
    }

    return 0;
}