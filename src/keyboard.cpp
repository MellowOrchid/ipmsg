#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include "user.h"
#include "public.h"
#include "keyboard.h"
#include "pack_unpack.h"
#include "IPMSG.H"
using std::cin, std::cout, std::cerr, std::string, std::vector, std::list;

sockaddr_in dest_addr;
keyboard::keyboard() {}

keyboard::~keyboard() {}

/** 键盘输入 */
int *keyboard::kb_scan()
{
    std::string ucmd;
    int recvBytes;
    while (1)
    {
        cout << "ipmsg> ";
        // 读取键盘输入，应当使用整行读取
        getline(cin, ucmd);

        if (ucmd == "users")
            users_cmd();
        else if (ucmd.find("sendto") != -1)
            sendto_cmd(ucmd);
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

    cout << "使用指南：\n";
    for (int i = 0; i < names.size(); i++)
        cout << names.at(i) << "：" << cmds.at(i) << '\n';
}

/** 显示用户 */
void keyboard::users_cmd()
{
    cout << "展示用户：\n";
    if (ulist.empty())
    {
        cout << "列表为空。\n";
        return;
    }

    for (auto &&i : ulist)
    {
        cout << "姓名：" << i.name << "\tIP：" << inet_ntoa(i.sin_addr) << '\n';
    }
}

/** 退出 */
void keyboard::exit_cmd()
{
    cout << "程序退出……\n";
}

void keyboard::sendto_cmd(string cmd)
{
    dest = cmd.substr(7);
    if (!hasUser(dest))
    {
        cerr << "该用户不存在。\n";
        return;
    }

    cout << "输入 `exit` 可退出交流\n";
    while (1)
    {
        cout << "发送给【" << dest << "】：";
        cin.getline(message, BUFFER_SIZE);
        if (!strcmp(message, "exit"))
        {
            cout << "与【" << dest << "】的交流结束\n";
            break;
        }
        coding(codingbuff, IPMSG_SENDMSG, message);
        result = sendto(udp_sock, codingbuff, strlen(codingbuff), 0,
                        (sockaddr *)&dest_addr, sizeof(dest_addr));
        if (result == -1)
        {
            // cout << "送信失败\n";
            cerr << "送信失败：" << strerror(errno) << '\n'
                 << "与【" << dest << "】的交流结束\n";
            break;
        }
    }
}

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
