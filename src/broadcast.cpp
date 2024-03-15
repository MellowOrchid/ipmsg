#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "write_log.h"
#include "broadcast.h"
#include "IPMSG.H"
#include "public.h"
#include "history.h"
#include "pack_unpack.h"
using std::cin, std::cout, std::string, std::cerr;

broadcast::broadcast() {}

broadcast::~broadcast() {}

/**
 * 发送广播信息
 * @param massage 要广播的信息
 */
void broadcast::send(const string &message)
{
    lmsg = "信息长度：";
    wlog::log(lmsg, message.length());
    // wlog::log(message.length());

    int result = sendto(udp_sock, message.c_str(), message.size(), 0,
                        (sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));

    lmsg = "套接字发送结果：";
    wlog::log(lmsg, result);
    if (result < 0)
    {
        lmsg = "套接字发送失败";
        wlog::log(lmsg);
        exit(3);
    }

    // wlog::log(result);
}

/**
 * 合并信息
 * @param buffer 目标字符串的指针
 * @param cmd IPMSG 协议中的指令代号
 * @param append 追加的信息
 * @deprecated 该函数已被重写于 `pack_unpack.h`
 */
void broadcast::coding(char *buffer, unsigned int cmd, char *append)
{
    time_t h;
    time(&h);

    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, myname, hname, cmd, append);

    lmsg = "合并后信息：";
    wlog::log(lmsg);
    wlog::log(buffer);
}

/** 执行广播 */
void broadcast::bc()
{
    optval = 1;

    lmsg = "端口号为：";
    wlog::log(lmsg, MSG_PORT);
    // wlog::log(MSG_PORT);
    cout << lmsg << MSG_PORT << '\n';

    // 创建 UDP 套接字
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        lmsg = "未能创建 UDP 套接字";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(1);
    }

    // 创建 TCP 套接字
    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0)
    {
        lmsg = "未能创建 TCP 套接字";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(1);
    }

    memset(&tcp_sock_addr, 0, sizeof(tcp_sock_addr));
    tcp_sock_addr.sin_family = AF_INET;
    tcp_sock_addr.sin_port = htons(MSG_PORT);
    tcp_sock_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcp_sock, (sockaddr *)&tcp_sock_addr, sizeof(sockaddr_in)) < 0)
    {
        lmsg = "未能绑定 TCP 套接字";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(2);
    }

    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    memset(&udp_sock_addr, 0, sizeof(udp_sock_addr));
    udp_sock_addr.sin_family = AF_INET;
    udp_sock_addr.sin_port = htons(MSG_PORT);
    udp_sock_addr.sin_addr.s_addr = INADDR_ANY;
    // 绑定套接字到端口
    if (bind(udp_sock, (sockaddr *)&udp_sock_addr, sizeof(sockaddr_in)) < 0)
    {
        lmsg = "未能绑定 UDP 套接字：";
        wlog::log(lmsg);
        wlog::log(strerror(errno));
        cerr << lmsg << strerror(errno) << '\n'; // 打印具体的错误信息
        exit(2);
    }

    if (listen(tcp_sock, 10))
    {
        lmsg = "TCP 监听错误";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(2);
    }

    inet_pton(AF_INET, br_ip, &udp_sock_addr.sin_addr);

    cmd::coding(buffer, IPMSG_BR_ENTRY, myname);
    send(buffer);

    inet_pton(AF_INET, myip, &udp_sock_addr.sin_addr);
    user u(udp_sock_addr.sin_addr, myname, hname);
    if (!ulist_impl.hasUser(udp_sock_addr.sin_addr))
        ulist.push_front(u);
    if (!history::has_user(myname))
        history::app_user(myname);
}
