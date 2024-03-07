#include <iostream>
#include <cstring>
#include <thread>
#include "public.h"
#include "keyboard.h"
#include "broadcast.h"
#include "udp_progress.h"
#include "tcp_send_file.h"
#include "write_log.h"
#include "file.h"
#include "cfg_init.h"
using std::cin, std::cout, std::string, std::thread;

int udp_sock;
int tcp_sock;
char myname[20], hname[20], ip[20], br_ip[20];
string lmsg;
list<user> ulist;
list<sendfile> send_file_list;
list<rcvfile> receive_file_list;
sockaddr_in tcp_sock_addr;
sockaddr_in udp_sock_addr;

int main()
{
    lmsg = "===== 程序开始 =====";
    wlog::log(lmsg);

    // 初始化本机信息
    cfg::init();

    keyboard kb;
    broadcast broadcast;
    udp_progress udp_prgs;

    broadcast.bc();
    kb.help_cmd();
    thread kb_thrd(&keyboard::kb_scan, &kb);
    thread udp_thrd(&udp_progress::udp_msg_process, &udp_prgs);
    thread tcp_thrd(&tcp_send_file::send_f);

    udp_thrd.join();
    kb_thrd.join();
    tcp_thrd.join();

    lmsg = "===== 程序结束 =====";
    wlog::log(lmsg);

    // system("pause");
    return 0;
}
