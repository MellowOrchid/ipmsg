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
    in_addr sin_addr;  // 用户 IP 地址
    sendfile();
    ~sendfile();
};

class rcvfile
{
private:
public:
    unsigned long num; // 文件序号
    long size;         // 文件大小
    long pkgnum;       // 包编号
    long utime;        // 修改时间
    char name[256];    // 文件名
    in_addr sin_addr;  // 用户 IP 地址
    rcvfile();
    ~rcvfile();
};

#endif
