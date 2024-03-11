#include <netinet/in.h>
#include <arpa/inet.h>

class contact_ipv6
{
private:
    int udp_sock6;
    int tcp_sock6;
    sockaddr_in6 udp_sock_addr6;
    sockaddr_in6 tcp_sock_addr6;
    char myip6[45];
    char dest_ip6[45];
    void init();
    void send_message();
    void send_file();

public:
    contact_ipv6();
    ~contact_ipv6();
    static void contact();
};
