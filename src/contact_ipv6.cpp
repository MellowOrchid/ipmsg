#include <iostream>
#include "contact_ipv6.h"
#include "public.h"
#include "write_log.h"
using std::cin, std::cout, std::cerr, std::string;

contact_ipv6::contact_ipv6() {}

contact_ipv6::~contact_ipv6() {}

/** 初始化本机信息 */
void contact_ipv6::init()
{
    cout << "请输入共有 IPv6 地址：";
    cin >> myip6;
    cout << "请输入目标 IPv6 地址：";
    cin >> dest_ip6;

    // 创建 UDP 套接字
    udp_sock6 = socket(AF_INET6, SOCK_DGRAM, 0);
    if (udp_sock6 < 0)
    {
        lmsg = "创建 IPv6 UDP 套接字失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return;
    }

    // 创建 TCP 套接字
    tcp_sock6 = socket(AF_INET6, SOCK_STREAM, 0);
    if (tcp_sock6 < 0)
    {
        lmsg = "创建 IPv6 TCP 套接字失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return;
    }

    memset(&udp_sock_addr6, 0, sizeof(udp_sock_addr6));
    udp_sock_addr6.sin6_family = AF_INET6;
    udp_sock_addr6.sin6_port = htons(MSG_PORT);
    inet_pton(AF_INET6, myip6, &udp_sock_addr6.sin6_addr);

    memset(&tcp_sock_addr6, 0, sizeof(tcp_sock_addr6));
    tcp_sock_addr6.sin6_family = AF_INET6;
    tcp_sock_addr6.sin6_port = htons(MSG_PORT);
    inet_pton(AF_INET6, myip6, &tcp_sock_addr6.sin6_addr);

    lmsg = "IPv6 套接字创建成功";
    wlog::log(lmsg);
    cout << lmsg << '\n';
}

/** 与目标用户建立联系 */
void contact_ipv6::contact()
{
    init();
}

void contact_ipv6::send_message()
{
    // 发送消息
}

void contact_ipv6::send_file()
{
    // 发送文件
}
