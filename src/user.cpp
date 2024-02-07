#include <cstring>
#include <netinet/in.h>
#include <list>
#include "user.h"
using std::string, std::list;

user::user() {}

user::user(in_addr &sin_addr, string name, string hostname)
{
    host = hostname;
    this->name = name;
    this->sin_addr = sin_addr;
}

user::~user() {}

bool user::operator==(const user &other) const
{
    return sin_addr.s_addr == other.sin_addr.s_addr;
}
