#ifndef _UDP_PRGS_
#define _UDP_PRGS_

#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "pack_unpack.h"
#include "userlist.h"
#include "public.h"
#include "IPMSG.H"
using std::cerr, std::cout;

class udp_progress
{
private:
    int udp_sock;
    char recvbuf[BUFF_SIZE];
    cmd cmd_obj;
    userlist ulist_impl;
    sockaddr_in udp_sock_addr;

public:
    udp_progress();
    ~udp_progress();
    void udp_msg_handle(cmd *msg, sockaddr_in *send_addr);
    void *udp_msg_process();
};

#endif
