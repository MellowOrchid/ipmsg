#include <iostream>
#include <cstring>
#include <thread>
#include "public.h"
#include "keyboard.h"
#include "broadcast.h"
#include "udp_progress.h"
#include "write_log.h"
#include "file.h"
using std::cin, std::cout, std::string, std::thread;

int udp_sock;
char myname[20], hname[20];
string lmsg;
list<user> ulist;
list<sendfile> send_file_list;
list<rcvfile> receive_file_list;
sockaddr_in udp_sock_addr;

int main()
{
    lmsg = "===== 程序开始 =====";
    wlog::log(lmsg);

    system("ifconfig");

    keyboard kb;
    broadcast broadcast;
    udp_progress udp_prgs;

    broadcast.bc();
    kb.help_cmd();
    thread kb_thrd(&keyboard::kb_scan, &kb);
    thread udp_thrd(&udp_progress::udp_msg_process, &udp_prgs);

    udp_thrd.join();
    kb_thrd.join();

    lmsg = "===== 程序结束 =====";
    wlog::log(lmsg);

    // system("pause");
    return 0;
}
