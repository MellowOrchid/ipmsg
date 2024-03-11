#include <iostream>
#include "contact_ipv6.h"
using std::cin, std::cout;

contact_ipv6::contact_ipv6() {}

contact_ipv6::~contact_ipv6() {}

void contact_ipv6::init()
{
    // 初始化本机信息
    cout << "请输入共有 IPv6 地址：";
    cin >> myip6;
    cout << "请输入目标 IPv6 地址：";
    cin >> dest_ip6;
    
}

void contact_ipv6::contact()
{
    // 与目标用户交流
}

void contact_ipv6::send_message()
{
    // 发送消息
}

void contact_ipv6::send_file()
{
    // 发送文件
}
