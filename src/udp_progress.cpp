#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "pack_unpack.cpp"
#include "public.h"
#include "IPMSG.H"
using std::cerr, std::cout;

class udp_progress
{
private:
    int udp_sock;
    char recvbuf[BUFF_SIZE];
    cmd cmd_obj;
    sockaddr_in udp_sock_addr;

public:
    udp_progress();
    ~udp_progress();
    void udp_msg_handle(struct cmd *msg, struct sockaddr_in *send_addr);
    void *udp_msg_process();
};

udp_progress::udp_progress() {}
udp_progress::~udp_progress() {}

// void udp_progress::udp_msg_handle(struct cmd *msg, struct sockaddr_in *send_addr)
// {
//     unsigned long tmp = 0;
//     // 接受到用户广播上线信息
//     if (GET_MODE(msg->cmdid) == IPMSG_BR_ENTRY)
//     {
//         // 没有此用户则添加此用户
//         // 根据sin_addr 判断
//         if (getUser(&send_addr->sin_addr) == NULL)
//         {
//             addUser(&send_addr->sin_addr, msg->name, msg->hostname);
//         }
//         char buff[BUFF_SIZE];
//         memset(buff, 0, sizeof(buff));
//         // 发送IPMSG_ANSENTRY信息
//         coding(buff, IPMSG_ANSENTRY, myname);
//         int sendBytes;
//         if ((sendBytes = sendto(udp_sock, buff, strlen(buff), 0,
//                                 (struct sockaddr *)send_addr, sizeof(struct sockaddr))) == -1)
//         {
//             printf("sendto fail\n");
//         }
//     }
//     // 接收到应答上线信息
//     if (GET_MODE(msg->cmdid) == IPMSG_ANSENTRY)
//     {
//         // 没有此用户则添加此用户
//         // 根据sin_addr 判断
//         if (getUser(&send_addr->sin_addr) == NULL)
//         {
//             addUser(&send_addr->sin_addr, msg->buf, msg->hostname);
//         }
//     }
//     // 接收到用户下线信息
//     if (GET_MODE(msg->cmdid) == IPMSG_BR_EXIT)
//     {
//         // 有此用户则删除此用户
//         struct user *logout_user;
//         if ((logout_user = getUser(&send_addr->sin_addr)) != NULL)
//         {
//             // 根据sin_addr 删除用户
//             delUser(send_addr->sin_addr);
//         }
//     }
//     // 接收到消息
//     if (GET_MODE(msg->cmdid) == IPMSG_SENDMSG)
//     {
//         char codingbuff[BUFF_SIZE];
//         if ((msg->cmdid & IPMSG_SENDCHECKOPT) == IPMSG_SENDCHECKOPT)
//         {
//             coding(codingbuff, IPMSG_RECVMSG, msg->id);
//             sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
//                    (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
//         }
//         printf("[recv msg from: %s :%s]#", msg->name, inet_ntoa(send_addr->sin_addr));
//         printf("%s\n", msg->buf);
//     }
//     // 接收到文件
//     if ((msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT)
//     {
//         char codingbuff[BUFF_SIZE];
//         coding(codingbuff, IPMSG_RECVMSG, msg->id);
//         sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
//                (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
//         struct rcvfile rcvfiles;
//         memset(&rcvfiles, 0, sizeof(rcvfiles));
//         rcvfiles.sin_addr = udp_sock_addr.sin_addr;
//         char *p1, *p2, i, *pp;
//         p1 = strrchr(recvbuf, 0);
//         printf("接收到包含文件信息的UDP数据包:%s \n", recvbuf);
//         // printf("接收到的数据包解析：%s\n",p1);
//         p2 = (p1 + 1);
//         // printf("接收到的数据包再解析：%s\n",p2);
//         sscanf(p2, "%lx:%[^:]:%lx", &rcvfiles.num, rcvfiles.name, &rcvfiles.size);
//         pp = strtok(p2, ":");
//         for (i = 0; i < strlen(pp); i++)
//         {
//             tmp = tmp * 10 + (*p2 - 0x30);
//             p2++;
//         }
//         rcvfiles.num = tmp;
//         printf("用户: %s向您发送文件：", inet_ntoa(udp_sock_addr.sin_addr));
//         printf("%s\n", rcvfiles.name);
//         add_rcvFile(&rcvfiles.sin_addr, rcvfiles.name, rcvfiles.num, rcvfiles.size);
//     }
// }

void udp_progress::udp_msg_handle(struct cmd *msg, struct sockaddr_in *send_addr) {}

void *udp_progress::udp_msg_process()
{
    unsigned addrLen = sizeof(udp_sock_addr);
    int recvbytes;
    const int PORT = 2425;
    struct sockaddr_in serverAddr, clientAddr;

    memset(&udp_sock_addr, 0, sizeof(udp_sock_addr));

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    cout << "UDP 套接字信息：" << udp_sock << '\n';
    if (udp_sock < 0)
    {
        cerr << "未能创建套接字。\n";
        return 0;
    }
    // 设置套接字选项以允许广播
    int broadcastEnable = 1;
    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        cerr << "未能设置套接字选项。\n";
        return 0;
    }
    // 设置服务器地址结构
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 绑定套接字到端口
    if (bind(udp_sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "未能绑定套接字\n";
        return 0;
    }
    while (1)
    {
        // 接收用户信息，接收广播信息和广播机器的 IP，不限源
        if ((recvbytes = recvfrom(udp_sock, recvbuf, sizeof(recvbuf), 0,
                                  (sockaddr *)&udp_sock_addr, &addrLen)) != -1)
        {
            recvbuf[recvbytes] = '\0';
            cout << "接收到 UDP 数据包：" << recvbuf;

            memset(&cmd_obj, 0, sizeof(cmd_obj));
            transcode(&cmd_obj, recvbuf, recvbytes);
            cout << "解析完命令开始执行\n";
            udp_msg_handle(&cmd_obj, &udp_sock_addr);
            cout << "执行完成\n\n";
        }
        else
            cout << "UDP接收失败\n";
    }
    return 0;
}
