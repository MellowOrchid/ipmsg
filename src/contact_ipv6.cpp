#include <iostream>
#include <thread>
#include <unistd.h>
#include "contact_ipv6.h"
#include "public.h"
#include "write_log.h"
#include "pack_unpack.h"
#include "IPMSG.H"
#include "history.h"
using std::cin, std::cout, std::cerr, std::string, std::thread;

contact_ipv6::contact_ipv6() {}

contact_ipv6::~contact_ipv6() {}

/** 初始化本机信息 */
void contact_ipv6::init()
{
    cout << "请输入本机公有 IPv6 地址：";
    cin >> myip6;
    cout << "请输入目标公有 IPv6 地址：";
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
    udp_sock_addr6.sin6_addr = in6addr_any;

    memset(&tcp_sock_addr6, 0, sizeof(tcp_sock_addr6));
    tcp_sock_addr6.sin6_family = AF_INET6;
    tcp_sock_addr6.sin6_port = htons(MSG_PORT);
    tcp_sock_addr6.sin6_addr = in6addr_any;

    lmsg = "IPv6 套接字创建成功";
    wlog::log(lmsg);
    cout << lmsg << '\n';

    bind(udp_sock6, (sockaddr *)&udp_sock_addr6, sizeof(udp_sock_addr6)); // 绑定地址和端口
    bind(tcp_sock6, (sockaddr *)&tcp_sock_addr6, sizeof(tcp_sock_addr6));
}

/** 与目标用户建立联系 */
void contact_ipv6::contact()
{
    init();
    thread udp_thread(&contact_ipv6::udp_process, this);
    thread tcp_thread(&contact_ipv6::send_file, this);
    send_message();

    udp_thread.join();
    tcp_thread.join();
}

void contact_ipv6::send_message()
{
    // 发送消息
    string msg;

    while (true)
    {
        cout << "请输入要发送的消息；\n"
             << "输入 \\sfile 发送文件，输入 \\gfile 接收文件，输入 \\exit 退出\n";
        getline(cin, msg);
        if (msg == "\\exit")
        {
            lmsg = "用户退出交流";
            wlog::log(lmsg);
            exit_cmd();
            break;
        }
        else if (msg == "\\sfile")
            pre_send_file();
        else if (msg == "\\gfile")
            get_file();
        else
            sendto(udp_sock6, msg.c_str(), msg.size(), 0, (sockaddr *)&udp_sock_addr6, sizeof(udp_sock_addr6));
    }
}

void contact_ipv6::get_file()
{
    // 接收文件
}

void contact_ipv6::udp_process()
{
    // UDP 进程
    char recvbuf[BUFF_SIZE];
    int recvbytes;
    unsigned addr_len = sizeof(udp_sock_addr6);
    cmd cmd_obj;

    while (true)
    {
        recvbytes = recvfrom(udp_sock6, recvbuf, sizeof(recvbuf), 0, (sockaddr *)&udp_sock_addr6, &addr_len);
        if (recvbytes == -1)
        {
            lmsg = "接收 UDP 数据包失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
            return;
        }
        recvbuf[recvbytes] = 3;
        recvbuf[recvbytes + 1] = 0;
        lmsg = "收到 UDP 消息：";
        wlog::log(lmsg);
        wlog::log(recvbuf);

        cmd::transcode(cmd_obj, recvbuf);
        lmsg = "解析完成，开始执行";
        wlog::log(lmsg);
        udp_msg_handle(&cmd_obj);
        lmsg = "执行完成";
        wlog::log(lmsg);

        // 接收到下线信息
        if (cmd_obj.cmdid == IPMSG_BR_EXIT && !strcmp(cmd_obj.name, myname))
            break;
    }
}

/**
 * 处理 UDP 数据包
 * @param msg 传来的数据包
 */
void contact_ipv6::udp_msg_handle(cmd *msg)
{
    // 接受到用户广播上线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_ENTRY)
    {
        strcpy(dest_name, msg->name);
        char buff[BUFF_SIZE];

        memset(buff, 0, sizeof(buff));
        // 发送 IPMSG_ANSENTRY 信息
        cmd::coding(buff, IPMSG_ANSENTRY, myname);
        int sendBytes = sendto(udp_sock6, buff, strlen(buff), 0,
                               (sockaddr *)&udp_sock_addr6, sizeof(sockaddr_in6));
        if (sendBytes == -1)
        {
            lmsg = "发送 IPMSG_ANSENTRY 失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
        }
    }

    // 接收到应答上线信息
    if (GET_MODE(msg->cmdid) == IPMSG_ANSENTRY)
        strcpy(dest_name, msg->name);

    // 接收到用户下线信息
    if (GET_MODE(msg->cmdid) == IPMSG_BR_EXIT)
        memset(dest_name, 0, sizeof(dest_name));

    // 接收到消息
    if (GET_MODE(msg->cmdid) == IPMSG_SENDMSG)
    {
        char codingbuff[BUFF_SIZE];
        if ((msg->cmdid & IPMSG_SENDCHECKOPT) == IPMSG_SENDCHECKOPT)
        {
            cmd::coding(codingbuff, IPMSG_RECVMSG, msg->id);
            sendto(udp_sock6, codingbuff, strlen(codingbuff), 0,
                   (sockaddr *)&udp_sock_addr6, sizeof(sockaddr_in));
        }

        // 防止空信息
        if (msg->buf[0] != 0)
        {
            cout << "\n接收到【" << dest_name << "】位于["
                 << dest_ip6 << "]的消息：\n"
                 << msg->buf << "\n\n"
                 << "请继续写：" << std::flush; // 刷新缓冲区，使其立即打印
            history::write_history(dest_name, dest_name, msg->buf);
        }
    }

    // 接收到文件
    if ((msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT)
    {
        unsigned long tmp = 0;
        char codingbuff[BUFF_SIZE];
        char recvbuf[BUFF_SIZE];
        char *csend, *pp;
        cmd::coding(codingbuff, IPMSG_RECVMSG, msg->id);
        sendto(udp_sock6, codingbuff, strlen(codingbuff), 0,
               (sockaddr *)&udp_sock_addr6, sizeof(sockaddr_in));
        rcvfile rcvd_file;

        lmsg = "接收到包含文件信息的 UDP 数据包：";
        wlog::log(lmsg);

        rcvd_file.sin6_addr = udp_sock_addr6.sin6_addr;
        csend = strrchr(recvbuf, 0) + 1;
        wlog::log(csend);

        pp = strtok(csend, ":");
        if (pp == NULL)
        {
            lmsg = "文件信息解析失败";
            wlog::log(lmsg);
            cerr << lmsg << '\n';
            return;
        }
        sscanf(csend, "%lx:%[^:]:%lx", &rcvd_file.num, rcvd_file.name, &rcvd_file.size);
        cout << "\n位于[" << dest_ip6 << "]的【"
             << msg->name << "】向您发送文件：" << rcvd_file.name << "\n\n"
             << "请继续写：" << std::flush; // 刷新缓冲区，使其立即打印

        for (long unsigned i = 0; i < strlen(pp); i++)
        {
            tmp = tmp * 10 + (*csend - 0x30);
            csend++;
        }
        rcvd_file.num = tmp;
        rcvd_file.whose = dest_name;
        receive_file_list.push_back(rcvd_file);
    }
}

void contact_ipv6::send_file()
{
    unsigned len;
    int recvbytes, cnct_sockt;
    char recvbuf[BUFF_SIZE];
    char sendbuf[BUFFER_SIZE];
    char rcv[1024];
    // 发送文件
    if (listen(tcp_sock6, 10))
    {
        lmsg = "TCP 监听错误";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(2);
    }

    len = sizeof(tcp_sock_addr6);
    while (true)
    {
        cnct_sockt = accept(tcp_sock6, (sockaddr *)&tcp_sock_addr6, &len);
        lmsg = "TCP 连接成功";
        wlog::log(lmsg);
        cout << '\n'
             << lmsg << '\n';

        recvbytes = recv(cnct_sockt, recvbuf, sizeof(recvbuf), 0);
        recvbuf[recvbytes] = '\003';
        recvbuf[recvbytes + 1] = 0;
        lmsg = "收到 TCP 消息：";
        wlog::log(lmsg);
        wlog::log(recvbuf);

        char version[50];
        sscanf(recvbuf, "%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\003]", version, rcv,
               cmd_obj.name, cmd_obj.hostname, &cmd_obj.cmdid, cmd_obj.buf);

        // 发文件
        if (GET_MODE(cmd_obj.cmdid) == IPMSG_GETFILEDATA)
        {
            ifs.open(file_path);
            if (!ifs.is_open())
            {
                lmsg = "未找到文件：";
                wlog::log(lmsg);
                wlog::log(file_path);
                cerr << lmsg << file_path << "\n\n";
                continue;
            }
            bzero(sendbuf, BUFFER_SIZE); // 缓冲区清零

            lmsg = "正在发送文件";
            wlog::log(lmsg);
            cout << lmsg << '\n';
            // 从文件读取数据并发送到客户端
            while (!ifs.eof())
            {
                ifs.read(sendbuf, BUFFER_SIZE);
                send(cnct_sockt, sendbuf, ifs.gcount(), 0);
            }

            ifs.close();

            lmsg = "发送文件完成：";
            wlog::log(lmsg);
            wlog::log(file_name);
            cout << lmsg << file_name << "\n\n"
                 << "请继续写：" << std::flush;
            close(cnct_sockt);

            history::write_history(cmd_obj.name, myname, lmsg, file_name);
        }

        if (cmd_obj.cmdid == OFFLINE)
            break;
    }
    lmsg = "TCP 监听结束";
    wlog::log(lmsg);
    cout << lmsg << '\n';
}

void contact_ipv6::exit_cmd()
{
    // 退出
    int sendBytes;
    char buff[BUFF_SIZE];
    cmd::coding(buff, IPMSG_BR_EXIT, myname);
    sendBytes = sendto(udp_sock6, buff, strlen(buff), 0, (sockaddr *)&udp_sock_addr6, sizeof(udp_sock_addr6));
    if (sendBytes == -1)
    {
        lmsg = "发送 IPMSG_BR_EXIT 信息失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
    }

    if (connect(tcp_sock6, (sockaddr *)&tcp_sock_addr6, sizeof(sockaddr_in)) < 0)
    {
        lmsg = "通知 IPv6 TCP 下线失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
    }

    string response = "1:0003:now:host:" + std::to_string(OFFLINE) + ":now";
    send(tcp_sock6, response.c_str(), response.size(), 0);

    close(udp_sock6);
    close(tcp_sock6);
}

void contact_ipv6::pre_send_file()
{
    // 发送文件前的准备
    cout << "请输入文件的绝对路径：";
    getline(cin, file_path);

    ifs.open(file_path);
    if (!ifs.is_open())
    {
        lmsg = "未找到文件：";
        wlog::log(lmsg);
        wlog::log(file_path);
        cerr << lmsg << file_path << "\n\n";
        return;
    }

    file_name = file_path.substr(file_path.find_last_of('/') + 1);
}
