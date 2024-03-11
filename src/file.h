#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <arpa/inet.h>
using std::string;

class sendfile
{
private:
public:
    unsigned long num; // 文件序号
    long size;         // 文件大小
    long pkgnum;       // 包编号
    long utime;        // 修改时间
    string name;       // 文件名
    string path;       // 绝对路径
    in_addr sin_addr;  // 用户 IP 地址
    sendfile();
    ~sendfile();
    bool operator==(const sendfile &other) const;
};

class rcvfile
{
private:
public:
    unsigned long num;  // 文件序号
    long size;          // 文件大小
    long pkgnum;        // 包编号
    long utime;         // 修改时间
    char name[256];     // 文件名
    string whose;       // 来源用户
    in_addr sin_addr;   // 用户 IPv4 地址
    in6_addr sin6_addr; // 用户 IPv6 地址
    rcvfile();
    ~rcvfile();
};

#endif
