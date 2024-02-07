#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include "user.h"
#include "public.h"
#include "keyboard.h"
#include "pack_unpack.h"
#include "IPMSG.H"
#include "filelist.h"
#include "write_log.h"

using std::cin, std::cout, std::cerr, std::string, std::vector, std::list, std::ofstream, std::ios;
namespace fs = std::filesystem;

sockaddr_in dest_addr;
filelist flist_impl;

keyboard::keyboard()
{
    cout << "广播 IP：";
    cin >> br_ip;
}

keyboard::~keyboard() {}

/** 键盘输入 */
int keyboard::kb_scan()
{
    int recvBytes;
    string ucmd;
    getchar(); // 读一个回车
    while (1)
    {
        cout << "ipmsg> ";
        // 读取键盘输入，应当使用整行读取
        getline(cin, ucmd);

        if (ucmd == "users")
            users_cmd();
        else if (ucmd.find("sendto") != -1)
            sendto_cmd(ucmd);
        else if (ucmd.find("getfile") != -1)
            getfile_cmd(ucmd);
        else if (ucmd == "RFL" || ucmd == "rfl")
            RFL_cmd();
        else if (ucmd == "exit")
        {
            exit_cmd();
            break;
        }
        else
            help_cmd();
    }

    return 0;
}

/** 帮助 */
void keyboard::help_cmd()
{
    vector<string> names, cmds;
    names.push_back("显示好友列表");
    names.push_back("给好友发送消息");
    names.push_back("显示发送文件列表");
    names.push_back("显示接收文件列表");
    names.push_back("发送文件");
    names.push_back("接收文件");
    names.push_back("退出程序");
    cmds.push_back("\t\tusers");
    cmds.push_back("\tsendto [好友名]");
    cmds.push_back("\tSFL");
    cmds.push_back("\tRFL");
    cmds.push_back("\t\tsendfile [好友名]");
    cmds.push_back("\t\tgetfile [文件名]");
    cmds.push_back("\t\texit");

    cout << "\n使用指南：\n";
    for (int i = 0; i < names.size(); i++)
        cout << names.at(i) << "：" << cmds.at(i) << '\n';
    cout << '\n';
}

/** 显示用户 */
void keyboard::users_cmd()
{
    cout << "\n展示用户：\n";
    if (ulist.empty())
    {
        cout << "列表为空。\n\n";
        return;
    }

    for (auto &&i : ulist)
        cout << "姓名：" << i.name << "\tIP：" << inet_ntoa(i.sin_addr) << '\n';

    cout << '\n';
}

/** 退出 */
void keyboard::exit_cmd()
{
    sockaddr_in exit_send_addr;
    int sendBytes;
    char buff[BUFF_SIZE], msg[2];
    memset(&buff, 0, sizeof(buff));

    // 设置套接字类型
    int set = 1;
    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &set, sizeof(int));
    memset(&exit_send_addr, 0, sizeof(exit_send_addr));
    exit_send_addr.sin_family = AF_INET;
    exit_send_addr.sin_port = htons(MSG_PORT);
    inet_pton(AF_INET, br_ip, &exit_send_addr.sin_addr);
    int len = sizeof(exit_send_addr);
    msg[0] = '0';
    msg[1] = 0;
    // 发送 IPMSG_BR_EXIT 信息
    cmd::coding(buff, IPMSG_BR_EXIT, msg);
    sendBytes = sendto(udp_sock, buff, strlen(buff), 0, (sockaddr *)&exit_send_addr, len);
    if (sendBytes == -1)
    {
        lmsg = "发送 IPMSG_BR_EXIT 信息失败";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
    }
    cout << "程序退出……\n";
}

/** 送信 */
void keyboard::sendto_cmd(string cmd)
{
    dest = cmd.substr(7);
    lmsg = "尝试交流：";
    wlog::log(lmsg);
    wlog::log(dest);
    if (!hasUser(dest))
    {
        lmsg = "该用户不存在。";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        return;
    }

    cout << "\n输入 `exit` 可退出交流\n";
    while (1)
    {
        cout << "发送给【" << dest << "】：";
        cin.getline(message, BUFFER_SIZE);
        if (!strcmp(message, "exit"))
        {
            cout << "与【" << dest << "】的交流结束\n\n";
            break;
        }
        cmd::coding(codingbuff, IPMSG_SENDMSG, message);
        result = sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
                        (sockaddr *)&dest_addr, sizeof(dest_addr));
        if (result == -1)
        {
            lmsg = "送信失败：";
            wlog::log(lmsg);
            wlog::log(strerror(errno));
            cerr << lmsg << strerror(errno) << '\n'
                 << "与【" << dest << "】的交流结束\n\n";
            break;
        }
    }
}

/** 根据用户名判断 */
bool keyboard::hasUser(string destU)
{
    bool exist = false;
    for (auto &&i : ulist)
    {
        if (i.name == destU)
        {
            memset(&dest_addr, 0, sizeof(dest_addr));
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_port = htons(MSG_PORT);
            dest_addr.sin_addr = i.sin_addr;
            exist = true;
            break;
        }
    }
    return exist;
}

/** 收到的文件 */
void keyboard::RFL_cmd()
{
    cout << "\n收到的文件：\n";
    if (receive_file_list.empty())
    {
        cout << "列表为空。\n\n";
        return;
    }

    for (auto &&i : receive_file_list)
        cout << "名称：" << i.name << "\t来自：" << inet_ntoa(i.sin_addr) << '\n';

    cout << '\n';
}

/** 接收文件 */
void keyboard::getfile_cmd(string cmd)
{
    string fileName = cmd.substr(8);
    string directoryPath = "./files";

    lmsg = "准备接收文件";
    wlog::log(lmsg);
    wlog::log(fileName);
    cout << lmsg << '\n';

    int sockfd, cnct_rst = 0, bytesRead, sendbytes;
    char sendbuf[BUFFER_SIZE], cmd_buf[BUFFER_SIZE];
    rcvfile file = flist_impl.find_file(fileName);
    if (file.size == 0)
    {
        lmsg = "文件未找到";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    // 连接
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(MSG_PORT);
    serverAddr.sin_addr.s_addr = file.sin_addr.s_addr;
    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    lmsg = "套接字结果：";
    wlog::log(lmsg);
    wlog::log(sockfd);
    if (sockfd < 0)
    {
        lmsg = "未能创建套接字";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    lmsg = "开始连接";
    wlog::log(lmsg);
    cout << lmsg << '\n';
    cnct_rst = connect(sockfd, (sockaddr *)&serverAddr, sizeof(sockaddr_in));
    lmsg = "连接结果：";
    wlog::log(lmsg);
    wlog::log(sockfd);
    if (cnct_rst == -1)
    {
        lmsg = "无法连接到对方";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    // 检查目录是否存在
    if (!fs::exists(directoryPath))
    {
        lmsg = "没有 `files` 这个目录";
        wlog::log(lmsg);

        // 目录不存在，创建目录
        try
        {
            fs::create_directory(directoryPath);
            lmsg = "新建成功";
            wlog::log(lmsg);
        }
        catch (const fs::filesystem_error &e)
        {
            lmsg = "创建目录失败";
            wlog::log(lmsg);
            wlog::log(e.what());
            cerr << lmsg << "：" << e.what() << '\n';
        }
    }

    ofstream ofs("files/" + fileName, ios::binary);
    if (!ofs.is_open())
    {
        lmsg = "无法写入文件";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    sprintf(cmd_buf, "%lx:%lx:%d", file.pkgnum, file.num, 0);
    cmd::coding(codingbuff, IPMSG_GETFILEDATA, cmd_buf);
    lmsg = "发送 IPMSG_GETFILEDATA 打包结果：";
    wlog::log(lmsg);
    wlog::log(cmd_buf);
    wlog::log(codingbuff);

    sendbytes = send(sockfd, codingbuff, strlen(codingbuff), 0);
    lmsg = "发送 IPMSG_GETFILEDATA 结果：";
    wlog::log(lmsg);
    wlog::log(sendbytes);
    if (sendbytes == -1)
    {
        lmsg = "发送 IPMSG_GETFILEDATA 失败";
        wlog::log(lmsg);
        return;
    }

    // 接收
    while ((bytesRead = recv(sockfd, sendbuf, BUFFER_SIZE, 0)) > 0)
        ofs.write(sendbuf, bytesRead);

    if (bytesRead == -1)
    {
        lmsg = "无法接收数据";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    // fs::path absolutePath = fs::absolute(fileName);

    lmsg = "成功接收数据并写入文件：";
    wlog::log(lmsg);
    // wlog::log(absolutePath);
    cout << lmsg << /*absolutePath <<*/ "\n\n";

    // 关闭套接字和文件
    close(sockfd);
    ofs.close();
}
