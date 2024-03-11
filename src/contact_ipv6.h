#ifndef _CONTACT_IPV6_
#define _CONTACT_IPV6_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include "pack_unpack.h"
using std::string, std::ifstream;

class contact_ipv6
{
private:
    int udp_sock6;
    int tcp_sock6;
    char myip6[45];
    char dest_ip6[45];
    char dest_name[50];
    sockaddr_in6 udp_sock_addr6;
    sockaddr_in6 tcp_sock_addr6;
    string file_path;
    string file_name;
    ifstream ifs;
    cmd cmd_obj;
    void init();
    void send_message();
    void send_file();
    void pre_send_file();
    void get_file();
    void udp_process();
    void udp_msg_handle(cmd *msg);
    void exit_cmd();

public:
    contact_ipv6();
    ~contact_ipv6();
    void contact();
};

#endif
