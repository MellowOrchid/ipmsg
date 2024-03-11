#include <netinet/in.h>
#include <arpa/inet.h>

class contact_ipv6
{
private:
    static int udp_sock6;
    static int tcp_sock6;
    static sockaddr_in6 udp_sock_addr6;
    static sockaddr_in6 tcp_sock_addr6;
    static char myip6[45];
    static char dest_ip6[45];
    static void init();
    static void send_message();
    static void send_file();

public:
    contact_ipv6();
    ~contact_ipv6();
    static void contact();
};
