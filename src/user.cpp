#include <cstring>
#include <netinet/in.h>
#include <list>
#include "user.h"
using std::string, std::list;

user::user() {}

/**
 * 包含用户信息的构造器
 * @param sin_addr 用户地址信息
 * @param name 用户名
 * @param hostname 用户的主机名
 */
user::user(in_addr &sin_addr, string name, string hostname)
{
    host = hostname;
    this->name = name;
    this->sin_addr = sin_addr;
}

user::~user() {}

/**
 * 根据地址信息判断两个用户是否相同
 */
bool user::operator==(const user &other) const
{
    return sin_addr.s_addr == other.sin_addr.s_addr;
}
