#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <netinet/in.h>
#include "user.h"
using std::string;

class user
{
private:
public:
    int num;          // 用户随机号
    string name;      // 用户名
    string host;      // 主机名
    in_addr sin_addr; // 用户IP地址
    user();
    user(in_addr &sin_addr, string name, string hostname);
    ~user();
    bool operator==(const user &other) const;
};

#endif
