#include <iostream>
#include <cstring>
#include <thread>
#include "public.h"
#include "keyboard.h"
#include "broadcast.h"
#include "udp_progress.h"
using std::cin, std::cout, std::string, std::thread;

int udp_sock;
list<user> ulist;

int main()
{
    keyboard kb;
    broadcast broadcast;
    udp_progress udp_prgs;

    broadcast.bc();
    kb.help_cmd();
    thread kb_thrd(&keyboard::kb_scan, &kb);
    thread udp_thrd(&udp_progress::udp_msg_process, &udp_prgs);

    udp_thrd.join();
    kb_thrd.join();
    // system("pause");
    return 0;
}
