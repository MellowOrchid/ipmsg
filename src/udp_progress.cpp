#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp_progress.h"
#include "pack_unpack.h"
#include "userlist.h"
#include "public.h"
#include "IPMSG.H"
#include "write_log.h"
#include "keyboard.h"
using std::cerr, std::cout;

udp_progress::udp_progress() {}
udp_progress::~udp_progress() {}

void udp_progress::udp_msg_handle(cmd *msg, sockaddr_in *send_addr)
{
    unsigned long tmp = 0;
    // 接受到用户广播上线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_ENTRY)
    {
        // 根据 sin_addr 判断该用户是否存在
        if (!ulist_impl.hasUser(send_addr->sin_addr))
        {
            ulist_impl.addUser(send_addr->sin_addr, msg->name, msg->hostname);
        }
        char buff[BUFF_SIZE];
        memset(buff, 0, sizeof(buff));
        // 发送 IPMSG_ANSENTRY 信息
        coding(buff, IPMSG_ANSENTRY, myname);
        int sendBytes = sendto(udp_sock, buff, strlen(buff), 0,
                               (struct sockaddr *)send_addr, sizeof(struct sockaddr));
        if (sendBytes == -1)
        {
            lmsg = "发送 IPMSG_ANSENTRY 失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
        }
    }
    // 接收到应答上线信息
    if (GET_MODE(msg->cmdid) == IPMSG_ANSENTRY)
    {
        /// 根据 sin_addr 判断该用户是否存在
        if (!ulist_impl.hasUser(send_addr->sin_addr))
        {
            ulist_impl.addUser(send_addr->sin_addr, msg->name, msg->hostname);
        }
    }
    // 接收到用户下线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_EXIT)
    {
        // 根据 sin_addr 判断该用户是否存在
        if (ulist_impl.hasUser(send_addr->sin_addr))
        {
            ulist_impl.delUser(send_addr->sin_addr);
        }
    }
    // 接收到消息
    if (GET_MODE(msg->cmdid) == IPMSG_SENDMSG)
    {
        char codingbuff[BUFF_SIZE];
        if ((msg->cmdid & IPMSG_SENDCHECKOPT) == IPMSG_SENDCHECKOPT)
        {
            coding(codingbuff, IPMSG_RECVMSG, msg->id);
            sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
                   (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
        }
        cout << "\n接收到【" << msg->name << "】位于["
             << inet_ntoa(send_addr->sin_addr) << "]的消息：\n"
             << msg->buf << "\n\n"
             << "请继续写：" << std::flush; // 刷新缓冲区，使其立即打印
    }
    // 接收到文件
    // if ((msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT)
    // {
    //     char codingbuff[BUFF_SIZE];
    //     coding(codingbuff, IPMSG_RECVMSG, msg->id);
    //     sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
    //            (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
    //     struct rcvfile rcvfiles;
    //     memset(&rcvfiles, 0, sizeof(rcvfiles));
    //     rcvfiles.sin_addr = udp_sock_addr.sin_addr;
    //     char *p1, *p2, i, *pp;
    //     p1 = strrchr(recvbuf, 0);
    //     printf("接收到包含文件信息的UDP数据包:%s \n", recvbuf);
    //     // printf("接收到的数据包解析：%s\n",p1);
    //     p2 = (p1 + 1);
    //     // printf("接收到的数据包再解析：%s\n",p2);
    //     sscanf(p2, "%lx:%[^:]:%lx", &rcvfiles.num, rcvfiles.name, &rcvfiles.size);
    //     pp = strtok(p2, ":");
    //     for (i = 0; i < strlen(pp); i++)
    //     {
    //         tmp = tmp * 10 + (*p2 - 0x30);
    //         p2++;
    //     }
    //     rcvfiles.num = tmp;
    //     printf("用户: %s向您发送文件：", inet_ntoa(udp_sock_addr.sin_addr));
    //     printf("%s\n", rcvfiles.name);
    //     add_rcvFile(&rcvfiles.sin_addr, rcvfiles.name, rcvfiles.num, rcvfiles.size);
    // }
}

void *udp_progress::udp_msg_process()
{
    unsigned addrLen = sizeof(udp_sock_addr);
    int recvbytes;
    sockaddr_in serverAddr;

    memset(&udp_sock_addr, 0, sizeof(udp_sock_addr));

    close(udp_sock);
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

    lmsg = "UDP 套接字信息：";
    wlog::log(lmsg);
    wlog::log(udp_sock);

    if (udp_sock < 0)
    {
        lmsg = "未能创建套接字。";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return 0;
    }
    // 设置套接字选项以允许广播
    int broadcastEnable = 1;
    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        lmsg = "未能设置套接字选项。";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return 0;
    }
    // 设置服务器地址结构
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(MSG_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字到端口
    if (bind(udp_sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        lmsg = "未能绑定套接字：";
        wlog::log(lmsg);
        wlog::log(strerror(errno));
        cerr << lmsg << strerror(errno) << '\n'; // 打印具体的错误信息
        return 0;
    }

    while (1)
    {
        // 接收用户信息，接收广播信息和广播机器的 IP，不限源
        if ((recvbytes = recvfrom(udp_sock, recvbuf, sizeof(recvbuf), 0,
                                  (sockaddr *)&udp_sock_addr, &addrLen)) != -1)
        {
            recvbuf[recvbytes] = 3;
            recvbuf[recvbytes + 1] = 0;
            lmsg = "接收到 UDP 数据包：";
            wlog::log(lmsg);
            wlog::log(recvbuf);

            memset(&cmd_obj, 0, sizeof(cmd_obj));
            transcode(cmd_obj, recvbuf, recvbytes);
            lmsg = "解析完成，开始执行";
            wlog::log(lmsg);
            udp_msg_handle(&cmd_obj, &udp_sock_addr);
            lmsg = "执行完成";
            wlog::log(lmsg);
        }
        else
        {
            lmsg = "UDP 接收失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
        }
    }
    return 0;
}
