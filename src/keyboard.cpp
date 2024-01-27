#include <iostream>
#include <cstring>
#include <vector>
// #include <arpa/inet.h>
// #include <iomanip>
// #include <ctime>
// #include "IPMSG.H"
// #include "userlist.h"
// #include "pack_unpack.h"
// #include "filelist.h"
// #include "public.h"
using std::cin, std::cout, std::string, std::vector;

class keyboard
{
private:
public:
    keyboard();
    ~keyboard();
    void kb_scan();
    void help_cmd();
    void users_cmd();
    void exit_cmd();
};

keyboard::keyboard() {}

keyboard::~keyboard() {}

/** 键盘输入 */
void keyboard::kb_scan()
{
    std::string ucmd;
    int recvBytes;
    while (1)
    {
        cout << "ipmsg> ";
        // 读取键盘输入
        cin >> ucmd;

        if (ucmd == "users")
            users_cmd();
        else if (ucmd == "exit")
        {
            exit_cmd();
            break;
        }
        else
            help_cmd();
    }
    // pthread_exit(NULL);

    // return 0;
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
    cout << "展示用户\n";
}

/** 退出 */
void keyboard::exit_cmd()
{
    cout << "程序退出……\n";
}
