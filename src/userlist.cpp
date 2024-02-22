#include <string>
#include <netinet/in.h>
#include "userlist.h"
#include "user.h"

userlist::userlist()
{
    ulist.clear();
}

userlist::~userlist() {}

/**
 * 判断某位用户是否在用户列表中
 * @param sin_addr 目标用户的地址信息
 * @return 若能找到匹配的地址，则返回真
 */
bool userlist::hasUser(in_addr &sin_addr)
{
    for (auto &&i : ulist)
        if (i.sin_addr.s_addr == sin_addr.s_addr)
            return true;
    return false;
}

/**
 * 将新用户添加到用户列表中
 * @param sin_addr 用户地址信息
 * @param name 用户名
 * @param hostname 用户的主机名
 */
void userlist::addUser(in_addr &sin_addr, string name, string hostname)
{
    user u(sin_addr, name, hostname);
    ulist.push_back(u);
}

/**
 * 删除特定用户
 * @param sin_addr 目标用户的地址信息
 */
void userlist::delUser(in_addr sin_addr)
{
    if (ulist.empty())
        return;

    for (auto &&i : ulist)
        if (i.sin_addr.s_addr == sin_addr.s_addr)
        {
            ulist.remove(i);
            break;
        }
}

/**
 * 判断某位用户是否在用户列表中
 * @param name 目标用户的用户名
 * @return 若能找到匹配的用户名，则返回真
 */
bool userlist::hasUser(string name)
{
    for (auto &&i : ulist)
        if (i.name == name)
            return true;
    return false;
}
