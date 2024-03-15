#include <iostream>
#include <fstream>
#include <unistd.h>
#include "cfg_init.h"
#include "public.h"
#include "write_log.h"
using std::cin, std::cout, std::cerr, std::fstream, std::ios, std::string;

cfg::cfg() {}

cfg::~cfg() {}

const string cfg_file = "cfg.ini";
static fstream cfg_stream;

/**
 * 配置文件检查与初始化
 */
void cfg::init()
{
    system("ifconfig");

    cfg_stream.open(cfg_file, ios::in);
    if (cfg_stream)
    {
        lmsg = "已有配置文件";
        wlog::log(lmsg);
        read_cfg();
    }
    else
    {
        lmsg = "无配置文件";
        wlog::log(lmsg);
        create_cfg();
    }
}

/**
 * 读取配置文件，当且仅当配置文件存在时调用
 */
void cfg::read_cfg()
{
    cfg_stream >> myname >> hname >> myip >> br_ip;
    cfg_stream.close();

    cout << "已有配置文件，信息如下：\n";
    cout << "用户名：" << myname << '\n'
         << "主机名：" << hname << '\n'
         << "本机 IP：" << myip << '\n'
         << "广播 IP：" << br_ip << '\n';
    cout << "是否使用该配置？接受请输 y，拒绝请输 n\n";
    char c;
    cin >> c;
    if (c == 'n' || c == 'N')
    {
        lmsg = "用户拒绝已有配置文件";
        wlog::log(lmsg);
        create_cfg();
    }
    else
    {
        lmsg = "用户接受已有配置文件";
        wlog::log(lmsg);
    }
}

/**
 * 创建配置文件
 */
void cfg::create_cfg()
{
    cfg_stream.open(cfg_file, ios::out);
    if (!cfg_stream)
    {
        lmsg = "创建配置文件失败，请检查权限";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(1);
    }
    else
    {
        gethostname(hname, sizeof(hname));
        cout << "本机 IP：";
        cin >> myip;
        cout << "广播 IP：";
        cin >> br_ip;
        cout << "用户名：";
        cin >> myname;
        cfg_stream << myname << '\n'
                   << hname << '\n'
                   << myip << '\n'
                   << br_ip << '\n';
    }
    cfg_stream.close();
}
