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
#include "file.h"
#include "history.h"
#include "encode_convert.h"
using std::cerr, std::cout;

udp_progress::udp_progress() {}
udp_progress::~udp_progress() {}

/**
 * 处理 UDP 数据包
 * @param msg 传来的数据包
 * @param send_addr 发送者的地址信息
 */
void udp_progress::udp_msg_handle(cmd *msg, sockaddr_in *send_addr)
{
    unsigned long tmp = 0;
    // 接受到用户广播上线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_ENTRY)
    {
        // 根据 sin_addr 判断该用户是否存在
        if (!ulist_impl.hasUser(send_addr->sin_addr))
            ulist_impl.addUser(send_addr->sin_addr, msg->name, msg->hostname);
        if (!history::has_user(msg->name))
            history::app_user(msg->name);

        char buff[BUFF_SIZE];
        memset(buff, 0, sizeof(buff));
        // 发送 IPMSG_ANSENTRY 信息
        cmd::coding(buff, IPMSG_ANSENTRY, myname);
        int sendBytes = sendto(udp_sock, buff, strlen(buff), 0,
                               (sockaddr *)send_addr, sizeof(sockaddr_in));
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
        // 根据 sin_addr 判断该用户是否存在
        if (!ulist_impl.hasUser(send_addr->sin_addr))
            ulist_impl.addUser(send_addr->sin_addr, msg->name, msg->hostname);

        if (!history::has_user(msg->name))
            history::app_user(msg->name);
    }

    // 接收到用户下线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_EXIT)
        // 根据 sin_addr 判断该用户是否存在
        if (ulist_impl.hasUser(send_addr->sin_addr))
            ulist_impl.delUser(send_addr->sin_addr);

    // 接收到消息
    if (GET_MODE(msg->cmdid) == IPMSG_SENDMSG)
    {
        char codingbuff[BUFF_SIZE];
        if ((msg->cmdid & IPMSG_SENDCHECKOPT) == IPMSG_SENDCHECKOPT)
        {
            cmd::coding(codingbuff, IPMSG_RECVMSG, msg->id);
            sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
                   (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr));
        }

        // 防止空信息
        if (msg->buf[0] != 0)
        {
            if (utf_convert == to_GBK)
            {
                // 先前要转换到 GBK，此处从 GBK 转换回 UTF-8
                string str = msg->buf;
                string convertedStr = encode_convert::convertEncoding(str, "UTF-8", "GBK");
                strncpy(msg->buf, convertedStr.c_str(), sizeof(msg->buf) - 1);
                msg->buf[sizeof(msg->buf) - 1] = '\0';
            }

            cout << "\n接收到【" << msg->name << "】位于["
                 << inet_ntoa(send_addr->sin_addr) << "]的消息：\n"
                 << msg->buf << "\n\n"
                 << "请继续写：" << std::flush; // 刷新缓冲区，使其立即打印
            history::write_history(msg->name, msg->name, msg->buf);
        }
    }

    // 接收到文件
    if ((msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT)
    {
        char codingbuff[BUFF_SIZE];
        char *csend, *pp;
        cmd::coding(codingbuff, IPMSG_RECVMSG, msg->id);
        sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
               (sockaddr *)&udp_sock_addr, sizeof(sockaddr_in));
        rcvfile rcvd_file;

        lmsg = "接收到包含文件信息的 UDP 数据包：";
        wlog::log(lmsg);

        rcvd_file.sin_addr = udp_sock_addr.sin_addr;
        csend = strrchr(recvbuf, 0) + 1;
        wlog::log(csend);

        sscanf(csend, "%lx:%[^:]:%lx", &rcvd_file.num, rcvd_file.name, &rcvd_file.size);
        cout << "\n位于[" << inet_ntoa(udp_sock_addr.sin_addr) << "]的【"
             << msg->name << "】向您发送文件：" << rcvd_file.name << "\n\n"
             << "请继续写：" << std::flush; // 刷新缓冲区，使其立即打印
        pp = strtok(csend, ":");

        if (pp == NULL)
        {
            lmsg = "文件信息解析失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
            return;
        }

        for (long unsigned i = 0; i < strlen(pp); i++)
        {
            tmp = tmp * 10 + (*csend - 0x30);
            csend++;
        }
        rcvd_file.num = tmp;
        rcvd_file.whose = msg->name;
        receive_file_list.push_back(rcvd_file);
    }
}

/**
 * 用于处理 UDP 信息的进程
 */
void udp_progress::udp_msg_process()
{
    unsigned addrLen = sizeof(udp_sock_addr);
    int recvbytes;

    while (true)
    {
        cmd cmd_obj;
        // 接收用户信息，接收广播信息和广播机器的 IP，不限源
        recvbytes = recvfrom(udp_sock, recvbuf, sizeof(recvbuf), 0,
                             (sockaddr *)&udp_sock_addr, &addrLen);
        if (recvbytes < 0)
        {
            lmsg = "UDP 接收失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
            return;
        }
        recvbuf[recvbytes] = 3;
        recvbuf[recvbytes + 1] = 0;
        lmsg = "接收到 UDP 数据包：";
        wlog::log(lmsg);
        wlog::log(recvbuf);

        cmd::transcode(cmd_obj, recvbuf);
        lmsg = "解析完成，开始执行";
        wlog::log(lmsg);
        udp_msg_handle(&cmd_obj, &udp_sock_addr);
        lmsg = "执行完成";
        wlog::log(lmsg);

        // 接收到下线信息
        if (cmd_obj.cmdid == IPMSG_BR_EXIT && !strcmp(cmd_obj.name, myname))
            break;
    }
    lmsg = "UDP 监听结束";
    wlog::log(lmsg);
    cout << lmsg << '\n';
}
