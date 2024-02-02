#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "broadcast.h"
#include "IPMSG.H"
#include "public.h"
using std::cin, std::cout, std::string, std::cerr;

broadcast::broadcast()
{
    cout << "本机 IP：";
    cin >> ip;

    cout << "用户名：";
    cin >> name;
}

broadcast::~broadcast()
{
    close(udp_sock);
}

/**
 * 发送广播信息
 * @param massage 要广播的信息
 */
void broadcast::send(const string &message)
{
    int result = sendto(udp_sock, message.c_str(), message.size(), 0,
                        (sockaddr *)&servaddr, sizeof(servaddr));
    cout << "套接字发送结果：" << result << '\n';
}

/** 合并信息 */
void broadcast::coding(char *buffer, unsigned int cmd, char *append)
{
    char hname[20];
    time_t h;

    time(&h);
    gethostname(hname, sizeof(hname));
    if (append == NULL)
        *append = '0';

    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, append, hname, cmd, append);
    cout << "合并后信息：" << buffer << '\n';
}

/** 执行广播 */
void broadcast::bc()
{
    optval = 1;

    cout << "端口号为：" << MSG_PORT << '\n';

    // 创建套接字
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        cerr << "未能创建套接字\n";
        exit(1);
    }

    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MSG_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    coding(buffer, IPMSG_BR_ENTRY, name);
    send(buffer);
}
